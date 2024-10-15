/**
 *  class Matchpool : stores transactions temporary
 *  before being included to the blockchain
 */
#ifndef MATCHPOOL_H
#define MATCHPOOL_H

#include "transaction.h"
#include "block.h"
#include <iostream>
#include <memory>
#include <queue>


#define MEM_POOL_SIZE 30

using namespace std;

class Matchpool
{
public:
    Matchpool();
    ~Matchpool(void);

    /**
     * Getters
     */
    int GetNumberOfTxs(void) const;

    /**
     * Add new transaction to the mempool
     */
    bool AddTransaction(shared_ptr<Transaction>);

    /**
     * Add vector of transaction to the mempool
     */
    void AddTransaction(const Block &block);

    /**
     * Remove mateched trade records form the mempool
     */
    void RemoveTransaction(shared_ptr<Transaction> transaction);

 

private:
    priority_queue<shared_ptr<Transaction>> transactions; // Transactions in the mempool
};

#endif // MEMPOOL_H
