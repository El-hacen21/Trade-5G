/**
 * class Blockchain : definition
 */

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "mempool.h"
#include "transaction.h"
#include "smart_contract.h"
#include <memory> // For std::unique_ptr

using namespace std;

namespace ns3
{

	class Blockchain
	{
	public:
		Blockchain(void);
		Blockchain(const string &serie); // Constructor by deserializing a json
										 // string
		virtual ~Blockchain(void);

		int GetTotalBlocks(void) const; // get the number of blocks in the blockchain

		int GetHeight(void) const; // the index of the last block

		vector<Block> GetMainChain(void); // returns the list blocks within the main chain

		unique_ptr<vector<Block>> GetBlockchain(void); // returns the list blocks

		/**
		 * Gets the current top block. If there are two blocks with the same height (siblings),
		 returns the one received first.
		 */
		const Block *GetCurrentTopBlock(void) const;

		/**
		 * Check if the block has been included in the blockchain.
		 */
		bool HasBlock(const Block &newBlock);

		/**
		 * Check if the size of the blockchain is lesser or equal to height. Return
		 * true if it is and false otherwise.
		 */
		bool HasBlock(int height);

		bool HasTransaction(const Transaction &tx);

		bool HasTransactions(const Block &block);

		/**
		 * Check if the the blockchain has a block with specified height and hash
		 * true if it is and false otherwise.
		 */
		bool HasBlock(int height, string blockHash) const;

		/**
		 * Return the block with the specified height and hash
		 * Should be called after HasBlock() to make sure that the block exists.
		 */
		Block ReturnBlock(int height, const string &blockHash);

		/**
		 * Return the block with the specified height. Return the first found block
		 * at this height.
		 * Should be called after HasBlock() to make sure that the block exists.
		 */
		Block ReturnBlock(int height);

		/**
		 * Return the block with the specified hash.
		 * Should be called after HasBlock(blockHash) to make sure that the block exists.
		 */
		Block *ReturnBlock(const string &blockHash);

		/**
		 * Verify if a block is valide
		 */
		bool ValidBlock(const Block &block, double &process_delay) const;

		int GetNbForks(void) const;

		int GetNbStaleBlocks(void) const;

		int GetLongestFork(void) const;

		Transaction* GetTransaction(const string &hash);

		/*
		 * Reorganize the blockchain in away that
		 * Helps to identify the main chain easily
		 */
		void UpdateMainchain(Mempool &mempool);

		/*
		 * Returns the number of mined blocks by a give Rsu
		 */
		int GetNbMinedBlock(int RsuId);
		/*
		 * Returns the size of the blockchain
		 */
		double BlockchainSizeInMB(int RsuId);

		/*
		 * Number of transactions in the main chain
		 */
		int NbConfirmedTransactions(void);

		/*
		 * Returns blockchain size - longest fork size
		 */
		vector<Block> GetConfirmedBlocks(int longest_fork);

		double BlockMeanSize(void);

		/**
		 * Gets the parent of a block
		 */
		Block *GetParent(const Block &block); // Get the parent of newBlock

		vector<Block> GetParents(const Block &block) const; // Get block parents

		/**
		 * Serialize the blockchain into a json format string.
		 */
		string Serialize() const;

		/**
		 * Adds a new block in the blockchain, return true if the block is successfuly added
		 */
		void AddBlock(const Block &newBlock);

		/**
		 * Deploys a smart contract
		 */
		void DeploySmartContract(const SmartContract &newSmartContract);

		/**
		 * Invokes a smart contract
		 */
		void InvokeSmartContract(const int contractID);

		
		/**
		 * Operator overload.
		 */
		friend ostream &operator<<(ostream &out, Blockchain &blockchain);

	private:
		int m_totalBlocks = 0;					  // total number of blocks including the genesis block
		vector<vector<Block>> m_blocks;			  // 2d vector containing all the blocks of the blockchain. (row->blockHeight, col->sibling blocks)
		map<int, SmartContract> m_smartContracts; // stores the smart contract

		int m_numbersOfStaleBlocks = 0; // the number of stale blocks
	};

} // namespace ns3

#endif // BLOCKCHAIN_H
