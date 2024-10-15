
#ifndef BLOCK_H
#define BLOCK_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "matched_transaction.h"

#define SHA256_SIZE 32
#define SHA1_SIZE 20

using namespace std;

class Transaction;
class MatchedTransaction;

class Block
{

public:
	// Constructors
	Block(int blockHeight, string blockHash, string blockPrevHash,
		  int RsuId, int blockSizeBytes = 0, double timeCreated = 0,
		  double timeReceived = 0, const map<uint32_t, string> &signers = {});
	Block();
	Block(const string &serie);		 // Constructor that deserializes and json string.
	Block(const Block &blockSource); // Copy constructor
	~Block(void);

public:
	/*
	 * Getters and Setters
	 */
	int GetBlockHeight(void) const;
	void SetBlockHeight(int blockHeight);

	int GetBlockNonce(void) const;
	void SetBlockNonce(int blockNonce);

	string GetBlockHash(void) const;
	void SetBlockHash(const string &blockHash);

	string GetBlockPrevHash(void) const;
	void SetBlockPrevHash(const string &blockPrevHash);

	string GetBlockMerkle(void) const;
	void SetBlockMerkle(const string &blockMerkle);

	int GetRsuId(void) const;
	void SetRsuId(int RsuId);

	int GetBlockSizeBytes(void) const;
	void SetBlockSizeBytes(int blockSizeBytes);

	double GetTimeCreated(void) const;
	void SetTimeCreated(double time);

	double GetTimeReceived(void) const;
	void SetTimeReceived(double time);

	double GetExpiredDemandsQuantity(void) const;
	void SetExpiredDemandsQuantity(double expiredQuantity);

	double GetExpiredOffersQuantity(void) const;
	void SetExpiredOffersQuantity(double expiredQuantity);

	void AddMatchedTransactions(shared_ptr<MatchedTransaction> transaction);
	vector<shared_ptr<MatchedTransaction>> GetMatchedTransactions(void) const;

	vector<shared_ptr<Transaction>> GetTransactions(void) const;
	void SetTransactions(const vector<Transaction> &transactions);
	void SetTransactions(vector<shared_ptr<Transaction>> transactions);

	const map<uint32_t, string> GetBlockSigners(void) const;
	const vector<uint32_t> GetBlockSignersIds(void) const;

	void SetBlockSigners(const map<uint32_t, string> &p_signers);

	/**
	 * Calculate the hash of the block from the given parameters and return it.
	 */
	string CalculateBlockHash(int blockHeight, int blockNonce, double timeCreated, int RsuId,
							  int blockSizeBytes, const string &blockMerkle, const string &blockPrevHashs) const;

	/**
	 * Calculate the hash of the block and return it.
	 */
	string CalculateBlockHash(void) const;

	/**
	 * Calculate the Merkle root
	 */
	string CalculateMerkle(void) const;

	/**
	 * Calculate the current size of the block and save it in m_blockSizeBytes
	 */
	void CalculateBlockSize(void);

	int GetBlockSize(void) const;
	/**
	 * Checks if the block is valide
	 */
	bool IsBlockValid(void) const;

	/**
	 * Checks if the block provided as the argument is the parent of this block object
	 */
	bool IsParent(const Block &block) const;

	/**
	 * Checks if the block provided as the argument is a child of this block object
	 */
	bool IsChild(const Block &block) const;

	/*
	 * Serialize the block into a json format string.
	 */
	string Serialize() const;

	/**
	 * Deserialize a json string of a microblock into a block whose
	 * transactions' data field is empty.
	 */
	// static Block DeSerializeMicroBlock(const string &m_data);

	// Serialize a micro block
	string SerializeMicroBlock() const;
	/*
	 * Add the signature to m_sig
	 */
	// void SignBlock(const signing &signature);

	/*
	 * Convert a block to micro block
	 */
	Block ToMicroBlock(void);

	/*
	 * Operator overloads
	 */
	Block &operator=(const Block &blockSource); // Assignment Constructor
	friend ostream &operator<<(ostream &out, const Block &block);
	friend bool operator==(const Block &block1, const Block &block2);
	bool operator<(const Block &blockSource) const;

private:
	int m_blockHeight;											   // The height of the block
	int m_blockNonce;											   // The nonce of the block
	string m_blockHash;											   // The block hash
	string m_blockPrevHash;										   // The hash of previous block
	string m_blockMerkle;										   // The block Merkle root
	int m_RsuId;												   // The id of the Rsu which mined this block
	int m_blockSizeBytes;										   // The size of the block in bytes
	double m_timeCreated;										   // The time the block was created
	vector<shared_ptr<Transaction>> m_transactions;				   // List of transactions
	vector<shared_ptr<MatchedTransaction>> m_matched_transactions; //
	double m_expired_demands_quantity;
	double m_expired_offers_quantity;
	// vector<signing> m_sig; // Block signaturess

	double m_timeReceived;			 // The time the block was received from the node
									 // the block to the receiving node (not serialize)
	map<uint32_t, string> m_signers; // Block signers with the leader at last position
};

#endif /* BLOCK_H */
