#include "experiment.h"
#include <ns3/core-module.h>
#include "ns3/point-to-point-module.h"
#include <ns3/mobility-module.h>
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/animation-interface.h"
#include <math.h>
#include <ns3/lte-module.h>
// #include "transaction.h"

using namespace std;
using namespace ns3;
// class Transaction;

Experiment::Experiment(int nb_robots, int scenario, int seed, int malicious, int commission, int duration)
{
	m_paramerters.scenario_type = scenario;
	// m_paramerters.number_of_txs = nb_txs;
	m_paramerters.seed = seed;
	// m_paramerters.DURATION_LAMBDA_EXP = DURATION_LAMBDA_EXP;
	m_paramerters.duration = duration;
	m_paramerters.nb_robots = nb_robots;
	m_paramerters.malicious = malicious;
	m_paramerters.commission = commission;
	m_nb_robots = nb_robots;
	m_duration = duration;

	// Nodes creation
	NodeContainer temp_nodes;
	temp_nodes.Create(nb_robots);
	int i;

	for (i = 0; i < nb_robots; i++)
		m_robots.Add(temp_nodes.Get(i));

	// Place nodes
	PlaceNodes("ns3::ConstantPositionMobilityModel");
	CreateNetwork();

	// Install application on the robots
	Install();
}

Experiment::~Experiment()
{
}

void Experiment::Init()
{
}

void Experiment::CreateWifi(string delayModel, string lossModel)
{
}

void Experiment::PlaceNodes(string mobilityModel)
{
	MobilityHelper robots_mobility;
	robots_mobility.SetMobilityModel(mobilityModel);
	robots_mobility.Install(m_robots);
}

void Experiment::CreateNetwork(void)
{

	PointToPointHelper pointToPoint2;
	pointToPoint2.SetDeviceAttribute("DataRate", StringValue("50Gbps"));
	pointToPoint2.SetChannelAttribute("Delay", StringValue("0ms"));

	// Install internet
	InternetStackHelper stack;
	stack.Install(m_robots);

	for (int i = 0; i < m_nb_robots; ++i)
	{
		for (int j = i; j < m_nb_robots; ++j)
		{
			if (i != j)
			{
				NetDeviceContainer newDevices = pointToPoint2.Install(m_robots.Get(i), m_robots.Get(j));
				m_devices.push_back(newDevices);
			}
		}
	}

	Ipv4AddressHelper ipv4;
	ipv4.SetBase("10.0.0.0", "255.255.255.0");
	for (const auto &a : m_devices)
	{
		Ipv4InterfaceContainer newInterfaces;
		newInterfaces.Add(ipv4.Assign(a));

		ipv4.NewNetwork();
	}
	// Ipv4InterfaceContainer robotsInterfaces = ippv4.Assign(m_devices);

	// Add the routing protocol
	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
}

Ipv4Address Experiment::GetAddress(uint32_t i)
{
	Ptr<Ipv4> ipv4 = m_robots.Get(i)->GetObject<Ipv4>();
	Ipv4Address addr = ipv4->GetAddress(1, 0).GetLocal();
	return addr;
}

void Experiment::Trace()
{
}

void Experiment::Run()
{
	Simulator::Stop(Seconds(m_duration));
	// AnimationInterface anim("animation.xml");  // animation
	// anim.SetMaxPktsPerTraceFile(90000);  // animation duration
	Simulator::Run();
	Simulator::Destroy();
}

void Experiment::Results()
{
	auto it = m_robotStat->begin();
	int countTxs_tmp = 0;
	double txFinality_tmp = 0;
	// for (auto block : it->second.blockchain.GetMainChain())
	// {
	// 	auto txs = block.GetTransactions();
	// 	for (auto &t : txs)
	// 	{
	// 		// diff += block.GetTimeCreated() - t.GetTimeCreated();
	// 		cout << " diff : " << block.GetTimeCreated() << " - " << t.GetTimeCreated() << endl;
	// 		// txFinality_tmp += (double)(block.GetTimeReceived() - t.GetTimeCreated());
	// 		countTxs_tmp++;
	// 	}
	// 	if (block.GetTransactions().size() > 0)
	// 		txFinality_tmp /= block.GetTransactions().size();
	// }
}

void Experiment::Install(void)
{
	// ApplicationContainer
	RobotHelper RobotHelper;

	for (int i = 0; i < m_nb_robots; ++i)
	{
		Ptr<Ipv4> ipv4 = m_robots.Get(i)->GetObject<Ipv4>();
		Ipv4Address addr = ipv4->GetAddress(1, 0).GetLocal();
		// Install robot application
		appRobots.Add(RobotHelper.Install(m_robots.Get(i), InetSocketAddress(addr), m_port, m_robots, m_paramerters, m_robotStat));
	}
}
