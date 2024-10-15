#include "transaction.h"
#include "block.h"
#include "utils.h"

using namespace ns3;

Block::Block(int blockHeight, string blockHash, string blockPrevHash,
			 int RsuId, int blockSizeBytes, double timeCreated,
			 double timeReceived, const map<uint32_t, string> &signers)
{
	m_blockHeight = blockHeight;
	m_blockHash = blockHash;
	m_blockPrevHash = blockPrevHash;
	m_RsuId = RsuId;
	m_blockSizeBytes = blockSizeBytes;
	m_timeCreated = timeCreated;
	m_timeReceived = timeReceived;
	m_signers = signers;
}

Block::Block(const string &serie)
{
	rapidjson::Document d;
	d.Parse(serie.c_str());
	m_blockHeight = d["0"].GetInt();
	m_blockNonce = d["1"].GetInt();
	hex2stream(d["2"].GetString(), m_blockHash);
	hex2stream(d["3"].GetString(), m_blockPrevHash);
	hex2stream(d["4"].GetString(), m_blockMerkle);
	m_RsuId = d["5"].GetInt();
	m_blockSizeBytes = d["6"].GetInt();
	m_timeCreated = d["7"].GetDouble();
	m_timeReceived = d["8"].GetDouble();

	if (d["9"].IsArray())
		for (auto &t : d["9"].GetArray())
			m_transactions.push_back(make_shared<Transaction>(t.GetString()));

	if (d["10"].IsArray())
		for (const auto &a : d["10"].GetArray())
			m_signers[a["0"].GetInt()] = a["1"].GetString();
}

Block::Block()
{
	*this = Block(0, "", "", -1, 0);
}

Block::Block(const Block &blockSource)
{
	*this = blockSource;
}

Block::~Block(void)
{
}

int Block::GetBlockHeight(void) const
{
	return m_blockHeight;
}

void Block::SetBlockHeight(int blockHeight)
{
	m_blockHeight = blockHeight;
}

int Block::GetBlockNonce(void) const
{
	return m_blockNonce;
}

void Block::SetBlockNonce(int blockNonce)
{
	m_blockNonce = blockNonce;
}

string Block::GetBlockHash(void) const
{
	return m_blockHash;
}

void Block::SetBlockHash(const string &blockHash)
{
	m_blockHash = blockHash;
}

string Block::GetBlockPrevHash(void) const
{
	return m_blockPrevHash;
}

void Block::SetBlockPrevHash(const string &blockPrevHash)
{
	m_blockPrevHash = blockPrevHash;
}

string Block::GetBlockMerkle(void) const
{
	return m_blockMerkle;
}

void Block::SetBlockMerkle(const string &blockMerkle)
{
	m_blockMerkle = blockMerkle;
}

int Block::GetRsuId(void) const
{
	return m_RsuId;
}

void Block::SetRsuId(int RsuId)
{
	m_RsuId = RsuId;
}

int Block::GetBlockSizeBytes(void) const
{
	return m_blockSizeBytes;
}

void Block::SetBlockSizeBytes(int blockSizeBytes)
{
	m_blockSizeBytes = blockSizeBytes;
}

double Block::GetTimeCreated(void) const
{
	return m_timeCreated;
}

void Block::SetTimeCreated(double time)
{
	m_timeCreated = time;
}

double Block::GetTimeReceived(void) const
{
	return m_timeReceived;
}

void Block::SetTimeReceived(double time)
{
	m_timeReceived = time;
}

vector<shared_ptr<Transaction>> Block::GetTransactions() const
{
	return m_transactions;
}

void Block::SetTransactions(vector<shared_ptr<Transaction>> transactions)
{
	for (const auto &transaction : transactions)
	{
		m_transactions.push_back(transaction);
	}
}

void Block::AddMatchedTransactions(shared_ptr<MatchedTransaction> transaction)
{
	// cout << "Adding : " << transaction->GetOfferTx() << " : " << transaction->GetDemandTx() << " :: " << transaction->GetQuantity() << endl;
	m_matched_transactions.push_back(transaction);
}

vector<shared_ptr<MatchedTransaction>> Block::GetMatchedTransactions() const
{
	return m_matched_transactions;
}

string Block::CalculateBlockHash(int blockHeight, int blockNonce, double timeCreated, int RsuId,
								 int blockSizeBytes, const string &blockMerkle, const string &blockPrevHash) const
{
	string hash(SHA256_SIZE, 0);

	string blockTextHeader = "" + to_string(blockHeight) + to_string(blockNonce) +
							 to_string(timeCreated) + to_string(RsuId) +
							 to_string(blockSizeBytes) + blockMerkle + blockPrevHash;

	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, blockTextHeader.data(), blockTextHeader.size());
	SHA256_Final((unsigned char *)hash.data(), &ctx);

	return hash;
}

string Block::CalculateBlockHash() const
{
	return CalculateBlockHash(GetBlockHeight(), GetBlockNonce(),
							  GetTimeCreated(), GetRsuId(), GetBlockSizeBytes(),
							  GetBlockMerkle(), GetBlockMerkle());
}

string Block::CalculateMerkle(void) const
{
	if (m_transactions.size() <= 0)
		return "";
	vector<string> txsHash; // Hash des transactions
	string temp1(SHA1_SIZE, 0), temp2(SHA1_SIZE, 0);

	for (const auto &tx : m_transactions)
	{
		txsHash.push_back(tx->GetTxHash());
	}

	while (txsHash.size() > 1)
	{

		if (txsHash.size() % 2 != 0)
		{
			txsHash.push_back(txsHash.back());
		}

		for (int itr = txsHash.size() - 1; itr >= 0; itr -= 2)
		{
			string current_node(SHA1_SIZE, 0);
			temp1 = txsHash[itr];
			temp2 = txsHash[itr - 1];
			for (int i = 0; i < SHA1_SIZE; i++)
			{
				current_node[i] = (temp1[i] ^ temp2[i]);
			}

			txsHash.pop_back();
			txsHash.pop_back();

			txsHash.push_back(current_node);
		}
	}

	return txsHash[0];
}

void Block::CalculateBlockSize()
{
}

int Block::GetBlockSize() const
{
	return 0;
}

bool Block::IsBlockValid() const
{

	if (GetBlockHash() != CalculateBlockHash())
	{

		return false;
	}

	return GetBlockMerkle() == CalculateMerkle();
}

bool Block::IsParent(const Block &block) const
{
	return block.GetBlockPrevHash() == this->GetBlockHash();
}

bool Block::IsChild(const Block &block) const
{
	return this->GetBlockPrevHash() == block.GetBlockHash();
}

const map<uint32_t, string> Block::GetBlockSigners(void) const
{
	return m_signers;
}

const vector<uint32_t> Block::GetBlockSignersIds(void) const
{
	vector<uint32_t> res;
	for (const auto &a : m_signers)
		res.push_back(a.first);
	return res;
}

void Block::SetBlockSigners(const map<uint32_t, string> &p_signers)
{
	m_signers = p_signers;
}

Block &Block::operator=(const Block &blockSource)
{
	m_blockHeight = blockSource.m_blockHeight;
	m_blockNonce = blockSource.m_blockNonce;
	m_blockHash = blockSource.m_blockHash;
	m_blockPrevHash = blockSource.m_blockPrevHash;
	m_blockMerkle = blockSource.m_blockMerkle;
	m_RsuId = blockSource.m_RsuId;
	m_blockSizeBytes = blockSource.m_blockSizeBytes;
	m_timeCreated = blockSource.m_timeCreated;
	m_timeReceived = blockSource.m_timeReceived;
	m_transactions = blockSource.m_transactions;
	m_matched_transactions = blockSource.m_matched_transactions;
	m_expired_demands_quantity = blockSource.m_expired_demands_quantity;
	m_expired_offers_quantity = blockSource.m_expired_offers_quantity;
	m_signers = blockSource.m_signers;
	return *this;
}

double Block::GetExpiredDemandsQuantity(void) const
{
	return m_expired_demands_quantity;
}

void Block::SetExpiredDemandsQuantity(double expiredQuantity)
{
	m_expired_demands_quantity = expiredQuantity;
}

double Block::GetExpiredOffersQuantity(void) const
{
	return m_expired_offers_quantity;
}

void Block::SetExpiredOffersQuantity(double expiredQuantity)
{
	m_expired_offers_quantity = expiredQuantity;
}

bool operator==(const Block &block1, const Block &block2)
{
	if (block1.GetBlockHeight() != block2.GetBlockHeight())
	{
		return false;
	}
	return block1.GetBlockHash() == block2.GetBlockHash();
}

ostream &operator<<(ostream &out, const Block &b)
{

	out << "Block(" << endl;
	out << " " << b.m_blockHash.data() << ",";
	out << " " << b.m_blockPrevHash.data() << ",";
	out << " " << b.m_blockHeight << ",";
	out << " " << b.m_blockNonce << ",";
	out << " " << b.m_RsuId << ",";
	out << " " << b.m_blockSizeBytes << ",";
	out << " " << b.m_timeCreated << ",";
	out << " " << b.m_blockMerkle.size() << ")";

	return out;
}

bool Block::operator<(const Block &blockSource) const
{
	return GetBlockHeight() < blockSource.GetBlockHeight();
}

string Block::Serialize() const
{
	rapidjson::Document d;
	rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
	rapidjson::Value value;

	d.SetObject();

	value = m_blockHeight;
	d.AddMember("0", value, d_alloc);

	value = m_blockNonce;
	d.AddMember("1", value, d_alloc);

	string hex;
	stream2hex(m_blockHash, hex);
	value.SetString(hex.data(), hex.size(), d_alloc);
	d.AddMember("2", value, d_alloc);

	hex = "";
	stream2hex(m_blockPrevHash, hex);
	value.SetString(hex.data(), hex.size(), d_alloc);
	d.AddMember("3", value, d_alloc);

	hex = "";
	stream2hex(m_blockMerkle, hex);
	value.SetString(hex.data(), hex.size(), d_alloc);
	d.AddMember("4", value, d_alloc);

	value = m_RsuId;
	d.AddMember("5", value, d_alloc);

	value = m_blockSizeBytes;
	d.AddMember("6", value, d_alloc);

	value = m_timeCreated;
	d.AddMember("7", value, d_alloc);

	value = m_timeReceived;
	d.AddMember("8", value, d_alloc);

	rapidjson::Value txs(rapidjson::kArrayType);
	for (auto &t : m_transactions)
	{
		string t_serie = t->Serialize();
		rapidjson::Value val(t_serie.c_str(), d_alloc);

		txs.PushBack(val, d_alloc);
	}
	d.AddMember("9", txs, d_alloc);

	rapidjson::Value signers(rapidjson::kArrayType);
	for (const auto &a : m_signers)
	{
		rapidjson::Value sig(rapidjson::kObjectType);
		value = a.first;
		sig.AddMember("0", value, d_alloc);

		value.SetString(a.second.c_str(), a.second.size(), d_alloc);
		sig.AddMember("1", value, d_alloc);

		signers.PushBack(sig, d_alloc);
	}
	d.AddMember("10", signers, d_alloc);

	rapidjson::StringBuffer serie;
	rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
	d.Accept(pointWriter);
	return serie.GetString();
}
