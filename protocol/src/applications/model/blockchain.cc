#include "blockchain.h"
#include <queue>
#include "utils.h"

namespace ns3
{

	Blockchain::Blockchain(void)
	{
		m_totalBlocks = 0;
		Block genesisBlock(0, "0", "", -1, 0, 0);
		AddBlock(genesisBlock);
	}

	Blockchain::~Blockchain(void)
	{
	}

	Blockchain::Blockchain(const string &serie)
	{
		rapidjson::Document d;
		d.Parse(serie.c_str());

		m_totalBlocks = d["totalBlocks"].GetInt();

		m_blocks = vector<vector<Block>>(0);

		for (auto &b : d["blocks"].GetArray())
		{
			int height = b["height"].GetInt();
			Block block(b["height"].GetString());

			while (m_blocks.size() < height)
				m_blocks.push_back(vector<Block>(0));

			m_blocks[height].push_back(block);
		}
	}

	int Blockchain::GetTotalBlocks(void) const
	{
		return m_totalBlocks;
	}

	int Blockchain::GetNbForks(void) const
	{
		int result = 0;
		for (auto it = m_blocks.begin(); it < m_blocks.end(); it++)
		{
			if (it->size() > 1)
			{
				result++;
			}
		}

		return result;
	}

	int Blockchain::GetNbStaleBlocks(void) const
	{
		return m_numbersOfStaleBlocks;
	}

	int Blockchain::GetLongestFork(void) const
	{

		int result = 0;
		queue<Block> path;

		for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
		{

			if (it->size() > 1)
			{

				for (auto i : *it)
				{ // for each

					map<Block, int> dist;
					dist[i] = 0;
					path.push(i);

					while (!path.empty())
					{

						Block v = path.front();
						path.pop();

						if (m_blocks.size() > v.GetBlockHeight() + 1)
						{
							vector<Block> childs = m_blocks[v.GetBlockHeight() + 1];

							if (childs.size() > 0)
								for (auto block : childs)
								{

									if (block == childs[0])
									{
									}
									else if (v.IsParent(block))
									{
										path.push(block);

										dist[block] = dist[v] + 1;
									}
								}
						}
					}

					// Calculate max
					int s = 1;
					for (auto it = dist.begin(); it != dist.end(); it++)
					{
						if (it->second > s)
						{
							s = it->second;
						}
					}

					result = max(s, result);
				}
			}
		}
		return result;
	}

	int Blockchain::NbConfirmedTransactions(void)
	{
		int result = 0;
		for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
		{

			result += (*it)[0].GetTransactions().size();
			// cout << "block size : " << result <<endl;
		}

		return result;
	}

	void Blockchain::DeploySmartContract(const SmartContract &newSmartContract)
	{
		cout << "Deploy the smart contract" << endl;
		m_smartContracts[newSmartContract.GetContractID()] = newSmartContract;
	}

	void Blockchain::InvokeSmartContract(const int contractID)
	{
		// cout << "invoke the smart contract" << contractID << endl;
		m_smartContracts[contractID].Execute();
	}

	const Block *Blockchain::GetCurrentTopBlock(void) const
	{

		return &(m_blocks[m_blocks.size() - 1][0]);
	}

	int Blockchain::GetHeight(void) const
	{
		return GetCurrentTopBlock()->GetBlockHeight();
	}

	bool Blockchain::HasBlock(const Block &newBlock)
	{

		// Warning : May cause bugs in the structure of the blockchain latter.
		// Computationnaly efficient but may be vulnerable to forged blocks.
		if (newBlock.GetBlockHeight() > GetCurrentTopBlock()->GetBlockHeight())
		{
			/* we haven't received it previously. */

			return false;
		}
		else
		{

			for (auto const &block : m_blocks[newBlock.GetBlockHeight()])
			{
				if (block == newBlock)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Blockchain::HasBlock(int height)
	{
		return m_blocks[height].size() > 0;
		// if (height > GetCurrentTopBlock()->GetBlockHeight())
		// {
		// 	/* we haven't received it previously. */
		// 	return false;
		// }
		// return true;
	}

	bool Blockchain::HasBlock(int height, string blockHash) const
	{
		// cout << this << endl;
		if (height > GetHeight())
		{
			/* we haven't received it previously. */
			return false;
		}
		else
		{
			for (const auto &it : m_blocks[height])
			{

				if (it.GetBlockHash() == blockHash)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Blockchain::HasTransaction(const Transaction &tx)
	{
		auto blockchain = this->GetMainChain();

		for (auto it = blockchain.begin(); it < blockchain.end(); it++)
		{
			for (auto &itr : it->GetTransactions())
			{
				if (*itr == tx)
				{
					return true;
				}
			}
		}

		return false;
	}

	Transaction* Blockchain::GetTransaction(const string &hash)
	{
		auto blockchain = this->GetMainChain();

		for (auto it = blockchain.begin(); it < blockchain.end(); it++)
		{
			for (auto &itr : it->GetTransactions())
			{
				if (itr->GetTxHash() == hash)
				{
					*itr;
				}
			}
		}

		return nullptr;
	}

	bool Blockchain::HasTransactions(const Block &block)
	{
		for (const auto &a : block.GetTransactions())
		{
			if (HasTransaction(*a))
				return true;
		}
		return false;
	}

	double Blockchain::BlockMeanSize(void)
	{
		double result = 0;

		for (auto it = m_blocks.begin() + 1; it != m_blocks.end(); it++)
		{

			result += (*it)[0].GetBlockSize() / (double)1000000;
			// cout << " Blocksize : " << (*it)[0].GetBlockSizeBytes() << endl;
		}

		return m_blocks.size() > 0 ? result / (double)(m_blocks.size() - 1) : 0;
	}

	int Blockchain::GetNbMinedBlock(int RsuId)
	{
		int count = 0;
		for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
		{
			if ((*it)[0].GetRsuId() == RsuId)
			{
				count++;
			}
		}
		return count;
	}

	double Blockchain::BlockchainSizeInMB(int RsuId)
	{
		double result = 0;

		for (auto it = m_blocks.begin() + 1; it != m_blocks.end(); it++)
		{
			result += (*it)[0].GetBlockSizeBytes();
			// cout << " BlocksizeInMB : " << (*it)[0].GetBlockSizeBytes() << endl;
		}

		return result / (double)1000000;
	}

	Block Blockchain::ReturnBlock(int height)
	{
		vector<Block>::iterator block_it;

		if (height <= GetHeight() && height >= 0)
		{

			for (block_it = m_blocks[height].begin(); block_it < m_blocks[height].end(); block_it++)
			{

				if (block_it->GetBlockHeight() == height)
				{
					return *block_it;
				}
			}
		}

		return Block(-1, "", "", -1, 0, 0);
	}

	Block Blockchain::ReturnBlock(int height, const string &blockHash)
	{

		vector<Block>::iterator block_it;

		if (height <= GetHeight() && height >= 0)
		{

			for (block_it = m_blocks[height].begin(); block_it < m_blocks[height].end(); block_it++)
			{

				if (block_it->GetBlockHeight() == height && block_it->GetBlockHash() == blockHash)
				{
					return *block_it;
				}
			}
		}

		return Block(-1, "", "", -1, 0, 0);
	}

	Block *Blockchain::ReturnBlock(const string &blockHash)
	{
		vector<Block>::iterator block_it;

		for (auto blocks : m_blocks)
		{
			for (block_it = blocks.begin(); block_it < blocks.end(); block_it++)
			{
				if (block_it->GetBlockHash() == blockHash)
				{
					return &(*(block_it));
				}
			}
		}

		return nullptr;
	}

	bool Blockchain::ValidBlock(const Block &block, double &process_delay) const
	{
		process_delay += blockValidationDelay;
		// Verify Merkle root
		if (block.CalculateMerkle() != block.GetBlockMerkle())
			return false;
		// Verify hash
		if (block.GetBlockHash() != block.CalculateBlockHash())
			return false;
		/* We don't verfy previous hash in this
		 * because of POW in case of forks
		 */

		return true;
	}

	Block *Blockchain::GetParent(const Block &block)
	{
		vector<Block>::iterator block_it;
		int parentHeight = block.GetBlockHeight() - 1;

		if (parentHeight > GetHeight() || parentHeight < 0)
			return nullptr;

		for (block_it = m_blocks[parentHeight].begin(); block_it < m_blocks[parentHeight].end(); block_it++)
		{

			if (block.IsChild(*block_it))
			{
				return &(*block_it);
			}
		}

		return nullptr;
	}

	vector<Block> Blockchain::GetParents(const Block &block) const
	{
		if (block.GetBlockHeight() == 0)
		{
			return {};
		}
		return m_blocks[block.GetBlockHeight() - 1];
	}

	void Blockchain::AddBlock(const Block &newBlock)
	{

		if (m_blocks.size() == 0)
		{ // Genesis
			vector<Block> newHeight(1, newBlock);
			m_blocks.push_back(newHeight);
		}
		else if (newBlock.GetBlockHeight() == GetCurrentTopBlock()->GetBlockHeight() + 1)
		{
			/*
			 * The new block has a new blockHeight, so have to create a new vector (row)
			 */

			vector<Block> newHeight(1, newBlock);
			m_blocks.push_back(newHeight);
		}
		else if (newBlock.GetBlockHeight() > GetCurrentTopBlock()->GetBlockHeight() + 1)
		{
			cout << "Add block error" << endl;
			exit(1);
		}
		else
		{
			/* The new block doesn't have a new blockHeight, so we have to add it in an existing row */
			m_numbersOfStaleBlocks++;
			m_blocks[newBlock.GetBlockHeight()].push_back(newBlock);
		}

		m_totalBlocks++;
	}

	ostream &operator<<(ostream &out, Blockchain &blockchain)
	{
		vector<Block>::iterator block_it;
		cout << "Blockchain " << endl;
		int height = 0; //
		for (auto blocks : blockchain.m_blocks)
		{
			cout << "index: " << height << endl;
			for (block_it = blocks.begin(); block_it < blocks.end(); block_it++)
			{

				cout << *block_it;
			}
			height++;
		}
		cout << endl;
		return out;
	}

	void Blockchain::UpdateMainchain(Mempool &mempool)
	{

		// for (int j = m_blocks.size() - 1; j > 0; j--)
		// {
		// 	vector<Block> *parents = &m_blocks[j - 1];
		// 	// bool isFirst = true;
		// 	for (int i = 0; i < parents->size(); i++)
		// 	{
		// 		// cout << " for updating " << endl;
		// 		if ((*parents)[i].IsParent(*m_blocks[j].begin()) && i == 0)
		// 			break;
		// 		else if ((*parents)[i].IsParent(*m_blocks[j].begin()) && i > 0)
		// 		{
		// 			parents->insert(parents->begin(), (*parents)[i]);
		// 			parents->erase(parents->begin() + i);

		// 		}

		// 	}
		// }

		// for (const auto &m : m_blocks)
		// 	for (auto it = m.begin() + 1; it < m.end(); it++)
		// 	{
		// 		for (const auto &tx : it->GetTransactions())
		// 			if (!HasTransaction(tx))
		// 				mempool.AddTransaction(tx);
		// 	}
		// for (const auto &a : m_blocks)
		// 	mempool.RemoveTransaction(*a.begin());
	}

	string Blockchain::Serialize() const
	{
		rapidjson::Document d;
		rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
		rapidjson::Value value;

		d.SetObject();
		rapidjson::Value blocks(rapidjson::kArrayType);
		for (int i = 0; i < m_blocks.size(); ++i)
		{
			for (auto b : m_blocks[i])
			{
				rapidjson::Value b_o(rapidjson::kObjectType);
				value = i;
				b_o.AddMember("height", value, d_alloc);
				string serie = b.Serialize();
				value.SetString(serie.c_str(), serie.size(), d_alloc);
				b_o.AddMember("block", value, d_alloc);

				blocks.PushBack(b_o, d_alloc);
			}
		}

		d.AddMember("blocks", blocks, d_alloc);
		value = m_totalBlocks;
		d.AddMember("totalBlocks", value, d_alloc);

		rapidjson::StringBuffer serie;
		rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
		d.Accept(pointWriter);

		return serie.GetString();
	}

	vector<Block> Blockchain::GetMainChain(void)
	{
		vector<Block> blocks;

		// Returns the longest chain

		for (auto it = m_blocks.begin() + 1; it != m_blocks.end(); it++)
		{
			
			auto block = *it->begin();
			
			blocks.push_back(block);
			
		}

		return blocks;
	}

	unique_ptr<vector<Block>> Blockchain::GetBlockchain()
	{
		auto blocks = make_unique<std::vector<Block>>(); // Using make_unique to create a vector of Block pointers

		// Assuming we're skipping the first block (genesis block)
		for (auto it = m_blocks.begin() + 1; it != m_blocks.end(); ++it)
		{
			blocks->push_back((*it->begin())); // Add the address of the current Block to the vector
		}

		return blocks; // Return the smart pointer to the vector of Block pointers
	}

	vector<Block> Blockchain::GetConfirmedBlocks(int longest_fork)
	{
		// auto longest_fork_size = GetLongestFork();

		if (longest_fork == 0)
			return GetMainChain();
		else
		{
			vector<vector<Block>> newVec(m_blocks.begin(), m_blocks.end() - longest_fork - 1);
			vector<Block> blocks;
			for (auto it = newVec.begin() + 1; it != newVec.end(); it++)
			{
				blocks.push_back(*it->begin());
			}

			return blocks;
		}
	}

} // namespace ns3
