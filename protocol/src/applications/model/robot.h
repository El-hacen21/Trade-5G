/**
 *  class Robot definition : defines Robot application
 *  Mainly generate hellos
 */

#ifndef Robot_H
#define Robot_H

#include "mypacket.h"
#include <ns3/applications-module.h>
// #include "hello.h"
#include "ns3/ipv4.h"
#include "blockchain.h"
#include "tendermint.h"
#include <memory>
#include "robot_statics.h"
#include <queue>
#include <random>

namespace ns3
{

	struct TransactionComparator
	{
		bool operator()(const std::shared_ptr<Transaction> &lhs, const std::shared_ptr<Transaction> &rhs) const
		{
			return *lhs < *rhs;
		}
	};

	class Address;
	class Socket;
	class Packet;

	class Robot : public Application
	{
	public:
		static TypeId GetTypeId(void);

		/**
		 * Constructors and destructor.
		 */
		Robot(void);
		virtual ~Robot(void);

		void SetUp(Address address, uint16_t port, NodeContainer robots, RobotParameter parameters, map<uint32_t, RobotStatics> *stats);

		void DoDispose(void);

		void StartApplication(void);

		void StopApplication(void);

		Ipv4Address GetIp4Address(const uint32_t &p_id);

		bool IsTendermintLeader(void);

		/*
		 *Sign message
		 */
		string Sign(const string &msg) const;

		void GenerateCrossTx(const string hash);

		pair<double, double> ComputeAveragePrice(priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> &pq);

		double ComputeMarketPriceDemands(const double market_mean, const double demands_q, const double offers_q) const;
		double ComputeMarketPriceOffers(const double market_mean, const double demands_q, const double offers_q) const;
		double ComputeTotalQuantity(priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> &pq);

	private:
		vector<uint32_t> GetCommittee(void); // returns k-1 the nearby BSs
		void Broadcast(const MyPacket &p_packet);
		void Multicast(const vector<uint32_t> &p_receivers, const MyPacket &p_packet);
		void Send(const MyPacket &p_packet, const uint32_t &p_id);
		void WriteUntilBufferFull(Ptr<Socket> sockt, uint32_t txSpace);

		/*This methods are automatically invoked given the updates on socket */
		void HandleRead(Ptr<Socket> socket);				   // when new data to read
		void HandleAccept(Ptr<Socket> s, const Address &from); // when tcp connection accepted
		void HandlePeerClose(Ptr<Socket> socket);			   // when close
		void HandlePeerError(Ptr<Socket> socket);			   // when error

		void printQueue(const std::priority_queue<std::shared_ptr<Transaction>,
												  std::vector<std::shared_ptr<Transaction>>,
												  ns3::TransactionComparator> &q);

		/**Read received data*/
		void Read(const string &token);

		void SendPropose(void);
		void SendPrevote(void);
		void SendPrecommit(void);

		void SendNilPrevote(void);
		void SendNilPrecommit(void);
		void GenerateTransaction(void);
		void GenerateD(const double currentTime);
		void GenerateO(const double currentTime);
		void GenerateRandomTransaction(double currentTime);
		void GenerateDemand(const string &hash, int quantity, double duration);
		void GenerateOffer(const string &hash, int quantity, double duration);
		static void MatchingLogic(Block *proposed_block, Robot *robot);
		void GenerateTransactionBasedOnRatio(int offerProbability, int totalProbability, double currentTime);
		void IncreaseResources(const int quality);
		void DecreaseResources(const int quality);
		// Function to calculate and log resource utilization
		void logResourceUtilization();
		void InvokeSmartContract(Block *block);

		// Function to schedule the logging event
		void scheduleLoggingEvent();

		double ComputeAdjustedPriceOffer(double currentTime, double P_initial, double P_market, double alpha, double beta);
		double ComputeAdjustedPriceDemand(double currentTime, double P_initial, double P_market, double alpha, double beta);

	private:
		uint16_t m_port;
		uint32_t m_robotId;							  // Id of a robot
		NodeContainer m_robots;						  // list of all robots
		vector<Ipv4Address> m_robotAddresses;		  // adresse des autre robots
		map<Ipv4Address, Ptr<Socket>> m_peersSockets; // peers associated sockets

		Ptr<Socket> m_socket; // listning socket
		Address m_address;	  // local address to bind to

		map<Ptr<Socket>, string> m_data; // data to send

		map<Address, std::string> m_bufferedData; // stores incomplete packets data

		Blockchain m_blockchain;
		Mempool m_mempool;
		Mempool m_expired_demands;
		Mempool m_expired_offers;
		priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> m_unmatched_demands;
		priority_queue<shared_ptr<Transaction>, vector<shared_ptr<Transaction>>, TransactionComparator> m_unmatched_offers;

		Tendermint m_tendermint;

		EventId scheduledTendermintNilVoteEvent;
		map<uint32_t, RobotStatics> *m_stats;
		RobotParameter m_paramerters;

		int m_resources;
		pair<double, int> suggestedOffer;

		default_random_engine m_generator;

		// double meanOfferPrice = 8.0;										 // Example mean price for offers
		// double meanDemandPrice = 10.0;										 // Example mean price for demands
		double priceStdDev = 0;												  // Standard deviation for price
		std::uniform_int_distribution<int> offerQuantityDistribution{1, 10};  // Quantity range for offers
		std::uniform_int_distribution<int> demandQuantityDistribution{1, 10}; // Quantity range for demands
		// std::exponential_distribution<double> lifetimeDistribution{0.01};	  // Lifetime distribution
		std::exponential_distribution<double> lifetimeDistribution{0.01};	  
		bool m_malicious = false;

		int count_offers = 0;
		int count_demands = 0;


		double m_c1=0;
		double m_c2=0;
	};

} // namespace ns3
#endif
