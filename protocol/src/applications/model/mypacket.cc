#include "mypacket.h"
#include <../rapidjson/document.h>
#include <../rapidjson/writer.h>
#include <../rapidjson/stringbuffer.h>

MyPacket::MyPacket(const char &packetType, const string &data, const uint32_t &source,
				   const string &sig, const int dataSize, double timeCreated, const char broadcast)
{
	m_packetType = packetType;
	m_data = data;
	m_packetBcst = broadcast;
	m_dataSize = dataSize;
	m_timeCreated = timeCreated;
	m_source = source;
	m_sig = sig;
}
MyPacket::MyPacket(){
	
}

MyPacket::~MyPacket()
{
}

MyPacket::MyPacket(const MyPacket &p)
{
	*this = p;
}

MyPacket::MyPacket(const string &serie)
{

	rapidjson::Document d;

	d.Parse(serie.c_str());

	if (!d.IsObject())
	{
		m_packetType = -1;
		return;
	}

	m_packetType = d["0"].GetInt();

	m_data = d["1"].GetString();

	m_dataSize = d["2"].GetInt();

	// m_packetBcst = d["packetBcst"].GetInt();

	m_timeCreated = d["3"].GetDouble();

	m_source = d["4"].GetInt();

	m_sig = d["5"].GetString();
}

MyPacket &MyPacket::operator=(const MyPacket &p)
{
	m_data = p.m_data;
	m_packetType = p.m_packetType;
	m_packetBcst = p.m_packetBcst;
	m_dataSize = p.m_dataSize;
	m_timeCreated = p.m_timeCreated;
	m_source = p.m_source;
	m_sig = p.m_sig;
	return *this;
}

bool operator==(const MyPacket &p, const MyPacket &p1)
{
	return p.m_data == p1.m_data &&
		   p.m_packetType == p1.m_packetType &&
		   p.m_packetBcst == p1.m_packetBcst &&
		   p.m_dataSize == p1.m_dataSize &&
		   p.m_timeCreated == p1.m_timeCreated &&
		   p.m_source == p1.m_source &&
		   p.m_sig == p1.m_sig;
}

string MyPacket::GetData(void) const
{
	return m_data;
}

int MyPacket::GetPacketType(void)
{
	return m_packetType;
}

int MyPacket::GetDataSize(void) const
{
	return m_dataSize;
}

void MyPacket::SetDataSize(const int &p_dataSize)
{
	m_dataSize = p_dataSize;
}

double MyPacket::GetTimeCreated(void)
{
	return m_timeCreated;
}

uint32_t MyPacket::GetSource(void) const
{
	return m_source;
}
string MyPacket::GetSig(void) const
{
	return m_sig;
}
ostream &operator<<(ostream &out, const MyPacket &p)
{
	cout << "Packet(" << p.m_packetType << ", " << p.m_data << ", " << p.m_timeCreated << ")" << endl;
}

string MyPacket::Serialize() const
{

	rapidjson::Document d;
	rapidjson::Document::AllocatorType &d_alloc = d.GetAllocator();
	rapidjson::Value value;

	d.SetObject();

	value = m_packetType;
	d.AddMember("0", value, d_alloc);

	value.SetString(m_data.data(), m_data.size(), d_alloc);
	d.AddMember("1", value, d_alloc);

	// value = m_packetBcst;
	// d.AddMember("packetBcst", value, d_alloc);

	value = m_dataSize;
	d.AddMember("2", value, d_alloc);

	value = m_timeCreated;
	d.AddMember("3", value, d_alloc);

	value = m_source;
	d.AddMember("4", value, d_alloc);

	value.SetString(m_sig.data(), m_sig.size(), d_alloc);
	d.AddMember("5", value, d_alloc);

	rapidjson::StringBuffer serie;
	rapidjson::Writer<rapidjson::StringBuffer> pointWriter(serie);
	d.Accept(pointWriter);

	return serie.GetString();
}
