/**
 *  class Mempool : stores transactions temporary
 *  before being included to the blockchain
 */
#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "transaction.h"
#include "block.h"
#include <iostream>
#include <memory>

#define MEM_POOL_SIZE 30

using namespace std;

class Mempool
{
public:
	Mempool();
	~Mempool(void);

	/**
	 * Getters
	 */
	int GetNumberOfTxs(void) const;

	vector<shared_ptr<Transaction>> GetTransactions(int nbFirst);

	/**
	 * Add new transaction to the mempool
	 */
	bool AddTransaction(shared_ptr<Transaction>);

	/**
	 * Returns a poiter to a transaction in the mempool
	 */
	Transaction GetTransaction(const string txHash);

	/**
	 * Add vector of transaction to the mempool
	 */
	void AddTransaction(const Block &block);

	/**
	 * Check if transaction is in the mempool,
	 * Called before adding transaction
	 */
	bool HasTransaction(shared_ptr<Transaction> tx);

	// Search Transaction by hash
	bool HasTransaction(const string txHash);

	/**
	 * Updating the mempool by deleting invalid transactions
	 * Free space for new transactions
	 */
	void RemoveTransaction(const Block &block);

	/**
	 * Updating the mempool of unmateched trade records
	 */
	void SortOrginizeTransactions(const Block &block);

	bool AddTransactionOrginize(shared_ptr<Transaction> tx, const double currentTime);

	/**
	 * Remove mateched trade records form the mempool
	 */
	void RemoveMatchedTransaction(shared_ptr<Transaction> transaction);

	double RemoveExpiredTransactions(const double currentTime);

	int CountMatched(void);

	/**
	 * Operator overloads
	 */
	friend ostream &operator<<(ostream &out, const Mempool &mempool);

private:
	vector<shared_ptr<Transaction>> transactions; // Transactions in the mempool
};

#endif // MEMPOOL_H
