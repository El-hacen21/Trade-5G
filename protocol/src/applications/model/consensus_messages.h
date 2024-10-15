#ifndef CONSENSUS_MESSAGES_H
#define CONSENSUS_MESSAGES_H

#include <../rapidjson/document.h>
#include <../rapidjson/writer.h>
#include <../rapidjson/stringbuffer.h>
#include <string>

using namespace std;

struct Propose
{
	Block block;
	uint32_t idProposer; // We assume that RSUs pks are known and ordered

	Propose(const Block &p_block, uint32_t &p_idProposer) : block(p_block), idProposer(p_idProposer)
	{
	}

	Propose()
	{
	}
	~Propose()
	{
	}

	Propose(const string &serie)
	{
		rapidjson::Document d;
		d.Parse(serie.c_str());
		block = Block(d["0"].GetString());
		idProposer = d["1"].GetInt();
	}

	string Serialise(void) const
	{
		rapidjson::Document d;
		rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
		rapidjson::Value value;

		d.SetObject();

		// Serialize block
		auto str = block.Serialize();
		value.SetString(str.c_str(), str.size(), d_alloc);
		d.AddMember("0", value, d_alloc);

		value = idProposer;
		d.AddMember("1", value, d_alloc);

		rapidjson::StringBuffer serie;
		rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
		d.Accept(pointWriter);
		return serie.GetString();
	}
};

struct PrevoteCommit
{ // Prevote and Precommit messages

	string hash; // block hash, size=0 if nil prevote or precommit
	uint32_t id;

	PrevoteCommit(const string &p_hash, const uint32_t &p_id) : hash(p_hash), id(p_id)
	{
	}

	PrevoteCommit()
	{
	}
	~PrevoteCommit()
	{
	}

	PrevoteCommit(const string &serie)
	{
		rapidjson::Document d;
		d.Parse(serie.c_str());
		hash = d["0"].GetString();
		id = d["1"].GetInt();
	}

	string Serialise(void) const
	{
		rapidjson::Document d;
		rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
		rapidjson::Value value;

		d.SetObject();

		value.SetString(hash.c_str(), hash.size(), d_alloc);
		d.AddMember("0", value, d_alloc);

		value = id;
		d.AddMember("1", value, d_alloc);

		rapidjson::StringBuffer serie;
		rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
		d.Accept(pointWriter);
		return serie.GetString();
	}
};

struct PrevoteCommitNil
{ // Prevote and Precommit messages

	uint32_t id;
	int height;		// block to create height
	uint32_t round; // tendermint consensus round

	PrevoteCommitNil(const uint32_t &p_id, const int &p_height, const uint32_t &p_round) : id(p_id), height(p_height), round(p_round)
	{
	}

	PrevoteCommitNil()
	{
	}
	~PrevoteCommitNil()
	{
	}

	PrevoteCommitNil(const string &serie)
	{
		rapidjson::Document d;
		d.Parse(serie.c_str());
		id = d["0"].GetInt();
		height = d["1"].GetInt();
		round = d["2"].GetInt();
	}

	string Serialise(void) const
	{
		rapidjson::Document d;
		rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
		rapidjson::Value value;

		d.SetObject();

		value = id;
		d.AddMember("0", value, d_alloc);

		value = height;
		d.AddMember("1", value, d_alloc);

		value = round;
		d.AddMember("2", value, d_alloc);

		rapidjson::StringBuffer serie;
		rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
		d.Accept(pointWriter);
		return serie.GetString();
	}
};

#endif