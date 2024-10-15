#include "transaction.h"
#include "utils.h"

using namespace std;
using namespace ns3;

Transaction::Transaction(const string &hash, const int source, const int type, const int resource,
						 const int quantity, const double duration,
						 const double price)
{
	m_txHash = hash;
	m_sourceId = source;
	m_type = type;
	m_resource = resource;
	m_quantity = quantity;
	m_duration = duration;
	m_price = price;

	// cout << " Type : " << m_type << "-" << m_resource << "-" << m_quantity << "-" << m_duration << "-" << m_price << endl;
}

Transaction::Transaction()
{
	m_txHash = "";
	m_timeCreated = 0;
	m_timeConfirmed = 0;
	m_sourceId = -1;

	string m_dataHash = "";
	m_data = vector<int>();
}

Transaction::~Transaction()
{
}

Transaction::Transaction(const Transaction &t)
{

	*this = t;
}

// Getters and Setters implementations
int Transaction::GetType() const
{
	return m_type;
}

void Transaction::SetType(const int type)
{
	m_type = type;
}

int Transaction::GetResource() const
{
	return m_resource;
}

void Transaction::SetResource(const int resource)
{
	m_resource = resource;
}

int Transaction::GetQuantity() const
{
	return m_quantity;
}

void Transaction::SetQuantity(const int quantity)
{
	m_quantity = quantity;
}

double Transaction::GetDuration() const
{
	return m_duration;
}

void Transaction::SetDuration(const double duration)
{
	m_duration = duration;
}

double Transaction::GetPrice() const
{
	return m_price;
}

void Transaction::SetPrice(const double price)
{
	m_price = price;
}

bool Transaction::GetIsMatched() const
{
	return m_isMatched;
}
void Transaction::SetIsMatched(const bool is_matched)
{
	m_isMatched = is_matched;
}

Transaction::Transaction(const string &serie)
{
	rapidjson::Document d;
	d.Parse(serie.data());
	m_txHash = d["0"].GetString();
	m_timeCreated = d["1"].GetDouble();
	m_sourceId = d["2"].GetInt();
	m_txSizeBytes = d["3"].GetInt();
	m_type = d["5"].GetInt();
	m_resource = d["6"].GetInt();
	m_quantity = d["7"].GetInt();
	m_duration = d["8"].GetDouble();
	m_price = d["9"].GetDouble();
	m_timeConfirmed = d["10"].GetDouble();
}

string Transaction::Serialize() const
{

	string ret;

	rapidjson::Document d;
	rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
	rapidjson::Value value;

	d.SetObject();

	value.SetString(m_txHash.c_str(), m_txHash.size(), d_alloc);
	d.AddMember("0", value, d_alloc);

	value = m_timeCreated;
	d.AddMember("1", value, d_alloc);

	value = m_sourceId;
	d.AddMember("2", value, d_alloc);

	value = m_txSizeBytes;
	d.AddMember("3", value, d_alloc);

	// Add the type attribute
	value = m_type;
	d.AddMember("5", value, d_alloc);

	// Add the resource attribute
	value = m_resource;
	d.AddMember("6", value, d_alloc);

	value = m_quantity;
	d.AddMember("7", value, d_alloc);

	value = m_duration;
	d.AddMember("8", value, d_alloc);

	value = m_price;
	d.AddMember("9", value, d_alloc);

	value = m_timeConfirmed;
	d.AddMember("10", value, d_alloc);

	rapidjson::StringBuffer serie;
	rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
	d.Accept(pointWriter);

	return serie.GetString();
}

string Transaction::GetTxHash(void) const
{
	return m_txHash;
}

double Transaction::GetTimeCreated() const
{
	return m_timeCreated;
}

void Transaction::SetTimeCreated(double timeCreated)
{
	m_timeCreated = timeCreated;
}

double Transaction::GetTimeConfirmed() const
{
	return m_timeConfirmed;
}
void Transaction::SetTimeConfirmed(double timeConfirmed)
{
	m_timeConfirmed = timeConfirmed;
}

int Transaction::GetSourceId() const
{
	return m_sourceId;
}

void Transaction::SetSourceId(const int sourceId)
{
	m_sourceId = sourceId;
}

string Transaction::GetDataHash()
{
	return m_dataHash;
}

void Transaction::SetDataHash(const string &dataHash)
{
	m_dataHash = dataHash;
}

vector<int> Transaction::GetData() const
{
	return m_data;
}

void Transaction::SetData(const vector<int> &data)
{
	m_data = data;
}

void Transaction::SetTxHash(const string &txHash)
{
	m_txHash = txHash;
}

string Transaction::CalculateTransactionHash() const
{
	string hash(SHA1_SIZE, 0);

	string header = "" + to_string(m_timeCreated) + to_string(m_sourceId);

	header += m_dataHash;

	SHA1((unsigned char *)m_data.data(), m_data.size(), (unsigned char *)hash.data());

	return hash;
}
Transaction &Transaction::operator=(const Transaction &t)
{

	m_txHash = t.m_txHash;
	m_timeCreated = t.m_timeCreated;
	m_timeConfirmed = t.m_timeConfirmed;
	m_sourceId = t.m_sourceId;
	m_txSizeBytes = t.m_txSizeBytes;
	m_dataHash = t.m_dataHash;
	m_data = t.m_data;

	return *this;
}

int Transaction::GetTxSizeBytes() const
{
	return m_txSizeBytes;
}

void Transaction::SetTxSizeBytes(int txSizeBytes)
{
	m_txSizeBytes = txSizeBytes;
}

void Transaction::CalculateTransactionSize()
{
	int current_size = 2 * sizeof(int);
	current_size += sizeof(double);
	// current_size += transactionHashSize;
	current_size += m_dataHash.size();
	current_size += m_data.size();

	m_txSizeBytes = current_size;
}

ostream &operator<<(ostream &out, const Transaction &t)
{
	if (t.GetType() == TRADE_TYPE::DEMAND)
	{
		out << "Demand(";
	}
	else if (t.GetType() == TRADE_TYPE::OFFER)
	{
		out << "Offer(";
	}
	else
	{
		out << "TX(";
	}
	out << "Hash: " << t.m_txHash << ", ";
	out << "Matched: " << t.m_isMatched << ", ";
	out << "Creation: " << t.m_timeCreated << ", ";
	out << "Quantity: " << t.m_quantity << ", ";
	out << "Duration: " << t.m_duration << ", ";
	out << "Price: " << t.m_price << ", ";
	out << ")";

	return out;
}

bool operator==(const Transaction &t1, const Transaction &t2)
{
	return t1.GetTxHash() == t2.GetTxHash();
}

// bool Transaction::operator<(const Transaction &t) const
// {
// 	// Compare trade types first
// 	if (GetType() != t.GetType())
// 	{
// 		return GetType() < t.GetType();
// 	}

// 	// If trade types are the same, proceed with specific comparisons
// 	if (GetType() == TRADE_TYPE::DEMAND)
// 	{
// 		// For demands: prioritize lower prices, then smaller quantities
// 		if (GetPrice() != t.GetPrice())
// 		{
// 			return GetPrice() > t.GetPrice();
// 		}
// 		if (GetQuantity() != t.GetQuantity())
// 		{
// 			return GetQuantity() > t.GetQuantity();
// 		}
// 	}
// 	else
// 	{
// 		// For offers: prioritize lower prices, then higher quantities
// 		if (GetPrice() != t.GetPrice())
// 		{
// 			return GetPrice() < t.GetPrice();
// 		}
// 		if (GetQuantity() != t.GetQuantity())
// 		{
// 			return GetQuantity() < t.GetQuantity();
// 		}
// 	}

// 	// If price and quantity are the same, compare by time confirmed, then time created, then transaction hash
// 	if (GetTimeConfirmed() != t.GetTimeConfirmed())
// 	{
// 		return GetTimeConfirmed() < t.GetTimeConfirmed();
// 	}
// 	if (GetTimeCreated() != t.GetTimeCreated())
// 	{
// 		return GetTimeCreated() < t.GetTimeCreated();
// 	}

// 	return GetTxHash() < t.GetTxHash();
// }

bool Transaction::operator<(const Transaction &t) const
{
	// Compare trade types first
	if (GetType() != t.GetType())
	{
		return GetType() < t.GetType();
	}

	// If trade types are the same, proceed with specific comparisons
	if (GetType() == TRADE_TYPE::DEMAND)
	{
		// For demands: prioritize higher prices, then larger quantities
		if (GetPrice() != t.GetPrice())
		{
			return GetPrice() < t.GetPrice(); // Inverser la logique ici
		}
		if (GetQuantity() != t.GetQuantity())
		{
			return GetQuantity() < t.GetQuantity(); // Inverser la logique ici pour prioriser les grandes quantités
		}
	}
	else
	{
		// For offers: prioritize lower prices, then smaller quantities
		// Cette partie reste inchangée car elle est déjà correcte
		if (GetPrice() != t.GetPrice())
		{
			return GetPrice() < t.GetPrice();
		}
		if (GetQuantity() != t.GetQuantity())
		{
			return GetQuantity() < t.GetQuantity();
		}
	}

	// If price and quantity are the same, compare by time confirmed, then time created, then transaction hash
	if (GetTimeConfirmed() != t.GetTimeConfirmed())
	{
		return GetTimeConfirmed() < t.GetTimeConfirmed();
	}
	if (GetTimeCreated() != t.GetTimeCreated())
	{
		return GetTimeCreated() < t.GetTimeCreated();
	}

	return GetTxHash() < t.GetTxHash();
}

bool Transaction::IsExpired(const double currentTime)
{
	return (m_timeCreated + m_duration) < currentTime;
}
