/**
 *  class Experiment definition
 *  In this class we set up the simulation:
 *  setup network topology
 *  setup applications
 *  run the simulator
 */
#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <ns3/wifi-module.h>
#include <ns3/internet-module.h>
#include <../helper/robot_helper.h>

using namespace ns3;
using namespace std;

class Experiment
{
public:
	Experiment(int nb_robots, int scenario, int seed, int malicious, int commission, int duration);
	~Experiment();

	/**
	 * Activate the trace mode to get pcap traces and routing tables of nodes.
	 */
	void Trace();

	/**
	 * Initialize the environment of the simulator.
	 */
	void Init();

	/**
	 * Deploy the nodes over an area in a grid topology.
	 */
	void PlaceNodes(string mobilityModel);

	/**
	 * Start the simulation.
	 */
	void Run();

	/**
	 * Setup the wifi interface of nodes (layer 2).
	 */
	void CreateWifi(string delayModel, string lossModel);

	/**
	 * Configure BSs topology
	 */
	void CreateNetwork(void);

	/**
	 * Setup the Ipv4 address of nodes (layer 3).
	 */
	void CreateAddressing();

	/**
	 * Setup the blockchain and users applications on nodes.
	 */
	void Install(void);

	void Results(void);

public:
	/**
	 * Return the Ipv4 address of node i.
	 */
	Ipv4Address GetAddress(uint32_t i);

private:
	NodeContainer m_robots;
	WifiHelper wifi;
	YansWifiPhyHelper wifiPhy;
	vector<NetDeviceContainer> m_devices;
	vector<Ipv4InterfaceContainer> m_interfaces;
	map<uint32_t, vector<uint32_t>> m_connections;
	map<uint32_t, vector<Ipv4Address>> m_connectionIps;
	map<uint32_t, Ipv4Address> m_robotsConnectionIps;

	uint32_t m_duration; // simulation duration

	ApplicationContainer appRobots; // container of robots applications

	uint32_t m_nb_robots;		  // number of robots
	const uint16_t m_port = 9077; //

	int m_seed;								  // seed
	map<uint32_t, RobotStatics> *m_robotStat; // statistic

	RobotParameter m_paramerters;
};

#endif
