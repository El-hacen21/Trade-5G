/**
 *  class Mypacket : Handles packets Serialization and Deserialization
 */
#ifndef MY_PACKET_H
#define MY_PACKET_H

#include <iostream>
#include <string>

using namespace std;

class MyPacket
{
public:
	static const char BROADCAST = 0; // Set the packet as a broadcast packet
	static const char UNICAST = 1;	 // Set the packet as an unicast packet

	static const char HELLO = 2; // Hello message

	static const char PROPOSE = 3;
	static const char PREVOTE = 4;
	static const char PRECOMMIT = 5;
	static const char NIL_PREVOTE = 6;
	static const char NIL_PRECOMMIT = 7;
	static const char TX = 8;

public:
	MyPacket(const char &packetType, const string &data, const uint32_t &source, const string &sig, const int dataSize, double m_timeCreated, const char broadcast = UNICAST);
	MyPacket(const MyPacket &p);
	MyPacket(const string &serie); // Constructor by deserializing a json string
	~MyPacket();
	MyPacket();

	/**
	 * Getters & Setters
	 */
	string GetData(void) const;
	int GetPacketType(void);
	int GetDataSize(void) const;
	void SetDataSize(const int &p_dataSize);
	double GetTimeCreated(void);
	uint32_t GetSource(void) const;
	string GetSig(void) const;
	/**
	 * Operator overload.
	 */
	MyPacket &operator=(const MyPacket &t);
	friend bool operator==(const MyPacket &p, const MyPacket &p1);
	friend ostream &operator<<(ostream &out, const MyPacket &t);

	/**
	 * Serialize the packet into a json string and return it.
	 */
	string Serialize() const;

private:
	int m_packetType;	  // Packets type
	int m_packetBcst;	  // Might be used to implement a broadcast protocol..
	string m_data;		  // Packet data
	int m_dataSize;		  // Packet data size
	double m_timeCreated; // Packet time of creation
	uint32_t m_source;	  // Packet creator
	string m_sig;		  // Packet signature for authenticity
};

#endif
