#include "mempool.h"

using namespace std;

Mempool::Mempool()
{
}

Mempool::~Mempool()
{
}

int Mempool::GetNumberOfTxs() const
{
	return transactions.size();
}

// vector<Transaction> Mempool::GetTransactions(int nbFirst)
// {
// 	vector<Transaction> selected_txs;
// 	int total_transactions = transactions.size();

// 	// Determine the number of transactions to retrieve
// 	int num_to_retrieve = std::min(nbFirst, total_transactions);

// 	// Get the transactions
// 	for (int i = 0; i < num_to_retrieve; ++i)
// 	{
// 		selected_txs.push_back(*transactions[i]);
// 	}

// 	return selected_txs;
// }

vector<shared_ptr<Transaction>> Mempool::GetTransactions(int nbFirst)
{
	vector<shared_ptr<Transaction>> selected_txs;
	int total_transactions = transactions.size();

	int num_to_retrieve = total_transactions;
	if (nbFirst > 0)
	{
		num_to_retrieve == std::min(nbFirst, total_transactions);
	}

	// Get the transactions
	for (int i = 0; i < num_to_retrieve; ++i)
	{
		selected_txs.push_back(transactions[i]);
	}

	return selected_txs;
}

bool Mempool::AddTransaction(shared_ptr<Transaction> tx)
{
	if (!HasTransaction(tx))
	{
		transactions.push_back(tx);
		return true;
	}
	return false;
}

bool Mempool::AddTransactionOrginize(shared_ptr<Transaction> tx, const double currentTime)
{
	// Check if transaction already exists
	if (!HasTransaction(tx))
	{
		// Insert transaction sorted by price
		auto it = lower_bound(transactions.begin(), transactions.end(), tx,
							  [](const auto &a, const auto &b)
							  {
								  return a->GetPrice() > b->GetPrice(); // Assuming higher price should come first
							  });
		transactions.insert(it, tx);
		return true;
	}
	return false;
}

double Mempool::RemoveExpiredTransactions(const double currentTime)
{
	double removedQuantity = 0;

	// Using an iterator to go through transactions as we might erase some
	auto it = transactions.begin();
	while (it != transactions.end())
	{
		// Check if the transaction is expired and matches the specificSourceId
		if ((*it)->GetTimeCreated() + (*it)->GetDuration() < currentTime)
		{

			// Erase the transaction and update the iterator
			it = transactions.erase(it);

			// Add the quantity of the removed transaction to the total
			removedQuantity += (*it)->GetQuantity();
		}
		else
		{
			++it; // Move to the next transaction if not erased
		}
	}

	return removedQuantity;
}

int Mempool::CountMatched(void)
{
	auto count = 0;
	for (const auto t : transactions)
	{
		if (t->GetIsMatched())
		{
			count++;
		}
	}
	return count;
}

void Mempool::AddTransaction(const Block &block)
{
	for (const auto &tx : block.GetTransactions())
	{
		if (!HasTransaction(tx))
			transactions.push_back(tx);
	}
}

bool Mempool::HasTransaction(shared_ptr<Transaction> tx)
{
	for (const auto &txx : transactions)
	{
		if (txx->GetTxHash() == tx->GetTxHash())
		{
			return true;
		}
	}
	return false;
}

bool Mempool::HasTransaction(const string txHash)
{

	for (const auto &tx : transactions)
	{
		if (tx->GetTxHash() == txHash)
		{
			return true;
		}
	}

	return false;
}

void Mempool::SortOrginizeTransactions(const Block &block)
{
	cout << "Orgnize records" << endl;
}

void Mempool::RemoveMatchedTransaction(shared_ptr<Transaction> transaction)
{
	// cout << "Remove Matched Transaction : " << transaction->GetTxHash() << endl;

	auto it = find_if(transactions.begin(), transactions.end(),
					  [&transaction](const shared_ptr<Transaction> &tx)
					  { return tx->GetTxHash() == transaction->GetTxHash(); });

	if (it != transactions.end())
	{
		transactions.erase(it);
	}
}

Transaction Mempool::GetTransaction(const string txHash)
{

	// for (auto tx : transactions)
	// {
	// 	if (tx->GetTxHash() == txHash)
	// 	{
	// 		return tx;
	// 	}
	// }

	cout << "Get transaction by hash" << endl;
	exit(0);
}

void Mempool::RemoveTransaction(const Block &block)
{
	for (const auto &tx : block.GetTransactions())
	{
		auto it = find_if(transactions.begin(), transactions.end(),
						  [&tx](const shared_ptr<Transaction> &mempool_tx)
						  {
							  return mempool_tx->GetTxHash() == tx->GetTxHash();
						  });

		if (it != transactions.end())
		{
			transactions.erase(it);
		}
	}
}

ostream &operator<<(ostream &out, const Mempool &mempool)
{
	out << "Mempool(";
	for (const auto &tx : mempool.transactions)
	{
		out << *tx << endl;
	}
	out << ")";

	return out;
}
