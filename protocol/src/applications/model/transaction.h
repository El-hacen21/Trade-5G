#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>

#define SHA1_SIZE 20

using namespace std;


class Transaction
{
public:
	Transaction(const string &hash, const int source, const int type, const int resource,
				const int quantity, const double duration,
				const double price);

	Transaction();
	virtual ~Transaction();

	Transaction(const string &serie); // Constructor by deserialization of a
									  // json string
	Transaction(const Transaction &t);

	string GetTxHash(void) const;
	void SetTxHash(const string &txHash);

	double GetTimeCreated() const;
	void SetTimeCreated(double timeCreated);

	double GetTimeConfirmed() const;
	void SetTimeConfirmed(double timeConfirmed);

	int GetSourceId() const;
	void SetSourceId(const int sourceId);

	string GetDataHash();
	void SetDataHash(const string &dataHash);

	vector<int> GetData() const;
	void SetData(const vector<int> &data);

	int GetTxSizeBytes() const;
	void SetTxSizeBytes(int txSizeBytes);

	void SetType(const int type);
	int GetType() const;

	int GetResource() const;
	void SetResource(const int resource);

	int GetQuantity() const;			  // return type to int
	void SetQuantity(const int quantity); // parameter type to int

	double GetDuration() const;				 // return type to double
	void SetDuration(const double duration); // parameter type to double

	double GetPrice() const;		   // return type to double
	void SetPrice(const double price); // parameter type to double

	bool GetIsMatched() const;
	void SetIsMatched(const bool is_matched);

	bool IsExpired(const double currentTime);

	/*
	 * Calculate the hash (SHA1 : 160 bits) of the transaction and return it as a string.
	 */
	string CalculateTransactionHash() const;

	/*
	 * Calculate the transaction size and save it in the m_txSizeBytes attribute.
	 */
	void CalculateTransactionSize();

	/*
	 * Serialize the transaction into a json string and return it.
	 */
	string Serialize() const;

	/*
	 * Operator overloads.
	 */
	Transaction &operator=(const Transaction &t);
	bool operator<(const Transaction &t) const;
	friend ostream &operator<<(ostream &out, const Transaction &t);
	friend bool operator==(const Transaction &t1, const Transaction &t2);

private:
	string m_txHash;		  // transaction hash
	double m_timeCreated;	  // The time the transaction was created
	double m_timeConfirmed;	  // The time the transaction was finalized
	int m_sourceId;			  // The id of the node that generated the transaction
	int m_txSizeBytes;		  // Size of transaction in bytes
	string m_dataHash;		  // Hash of the data contained in the transaction
	vector<int> m_data;		  // Content of the transaction
	int m_type;				  // Type of the trade
	int m_resource;			  // Resource being traded
	int m_quantity;			  // Quantity of the resource
	double m_duration;		  // Duration of the trade
	double m_price;			  // Price of the trade
	bool m_isMatched = false; //
};

#endif // TRANSACTION_H
