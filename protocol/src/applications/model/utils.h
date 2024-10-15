/**
 *  Utils : In this file you will find some utils methods
 *  And implemented protocols parameters
 */

#ifndef UTILS_H
#define UTILS_H
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <openssl/sha.h> // hash
#include "ns3/simulator.h"
#include <../rapidjson/document.h>
#include <../rapidjson/writer.h>
#include <../rapidjson/stringbuffer.h>
#include <bitset>
#include <time.h>
#include <sys/time.h>
#include <ns3/vector.h>
#include <chrono>
#include <queue>

#define SHA1_SIZE 20

using namespace std;
using namespace ns3;

/********CONSTANTS***********/
// static const int tcpSegSize = 1024;
/********CONSTANTS***********/

inline double
GetSeconds(void)
{
	return (double)Simulator::Now().GetSeconds();
};

inline string dump(const char *data, int size)
{
	ostringstream buf;
	// buf << "0x";
	for (int i = 0; i < size; ++i)
		buf << setfill('0') << setw(2) << hex << (unsigned int)((unsigned char)data[i]);
	return buf.str();
};

inline string toBinary(const string &myString, const int size)
{
	string binaryString = "";

	for (size_t i = 0; i < myString.size(); ++i)
	{
		binaryString += bitset<8>(myString.c_str()[i]).to_string();
		if (binaryString.size() >= size)
			break;
	}

	return binaryString.substr(0, size);
};

// Convert string of chars to its representative string of hex numbers
inline void stream2hex(const string &str, string &hexstr, bool capital = false)
{
	hexstr.resize(str.size() * 2);
	const size_t a = capital ? 'A' - 1 : 'a' - 1;

	for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF)
	{
		hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
		hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
	}
};

// Convert string of hex numbers to its equivalent char-stream
inline void hex2stream(const string &hexstr, string &str)
{
	str.resize((hexstr.size() + 1) / 2);

	for (size_t i = 0, j = 0; i < str.size(); i++, j++)
	{
		str[i] = (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) << 4, j++;
		str[i] |= (hexstr[j] & '@' ? hexstr[j] + 9 : hexstr[j]) & 0xF;
	}
};

inline vector<string> tokenize(const string &s, const string &delimiters = " ")
{
	vector<string> tokens;

	// Skip delimiters at beginning
	auto lastPos = s.find_first_not_of(delimiters, 0);

	// Find the first "non-delimiter"
	auto pos = s.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector
		tokens.push_back(s.substr(lastPos, pos - lastPos));

		// Find next word
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}

	return tokens;
};

// Serialize into a json array a buffer of char
inline rapidjson::Value SerializeCharBuffer(const string &buffer,
											rapidjson::Document::AllocatorType &d_alloc)
{
	rapidjson::Value value(rapidjson::kArrayType);

	for (int i = 0; i < buffer.size(); ++i)
		value.PushBack(buffer[i], d_alloc);

	return value;
};

// Deserialize a json array into a buffer of char
inline string DeserializeCharBuffer(rapidjson::Value value)
{
	string ret(value.Capacity(), 0);
	int i = 0;
	for (auto &c : value.GetArray())
		ret[i++] = c.GetInt();

	return ret;
};

inline string HashDataBuffer(const string &data)
{
	string hash(SHA1_SIZE, 0);
	SHA1((unsigned char *)data.data(), data.size(), (unsigned char *)hash.data());
	return hash;
};

static double GetWallTime()
{
	struct timeval time;
	if (gettimeofday(&time, NULL))
	{
		//  Handle error
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
};

/*----------Schnoor sig-------------*/
inline map<uint32_t, double> ReadBench(const string &file)
{
	ifstream infile(file);
	uint32_t a;
	double b;
	map<uint32_t, double> out;
	while (infile >> a >> b)
	{
		out[a] = b;
	}

	infile.close();
	return out;
};


static const double sign_delay=0.01;
static const double verify_delay=0.01;


static const int nb_txs_per_block = 100;

//----------CONSTANTs-------------*/
static const int transactionSize = 800; // average size (Bytes) of transactions
// static const double blockValidationDelay = ReadDouble("../bench/block_validation_delay.txt");
static const double blockValidationDelay = 0;

//*************Tendermint consensus parameters************
//  How long we wait for a proposal block before prevoting nil
const static float timeout_propose = 1;
//  How much timeout_propose increases with each round
const static float timeout_propose_delta = 0.1;
// How long we wait after receiving +2/3 prevotes for “anything” (ie. not a single block or nil)
const static float timeout_prevote = 0.5;
// How much the timeout_prevote increases with each round
const static float timeout_prevote_delta = 0.1;
// How long we wait after receiving +2/3 precommits for “anything” (ie. not a single block or nil)
const static float timeout_precommit = 0.5;
// How much the timeout_precommit increases with each round
const static float timeout_precommit_delta = 0.1;
/* How long we wait after committing a block, before starting on the new
height (this gives us a chance to receive some more precommits, even
though we already have +2/3). */
const static float timeout_commit = 0.5;

//*****************************************//

const static float slice_affectation_delay = 0.25;

enum ScenarioType
{
	HIGH_DEMAND_LOW_OFFER,
	RANDOM,
	LOW_DEMAND_HIGH_OFFER,
	
	INCREASING_DEMAND_CONSTANT_OFFER,
	CONSTANT_DEMAND_INCREASING_OFFER
};

enum TRADE_TYPE
{
	DEMAND,
	OFFER
};

enum SMARTCONTRACTS
{
	MATCHING,
};

enum RESSOURCE_TYPE
{
	BANDWIDTH,
};

#endif