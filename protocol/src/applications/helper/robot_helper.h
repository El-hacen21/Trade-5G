/**
 * class Robot helper definition:
 * Helps install Robot application in a node
 */
#ifndef Robot_HELPER_H
#define Robot_HELPER_H

// #include "ns3/node.h"
#include "ns3/object-factory.h"
// #include "ns3/ipv4-address.h"
// #include "ns3/vector.h"
#include <ns3/node-container.h>
#include "../model/robot.h"

namespace ns3
{

	class RobotHelper
	{
	public:
		RobotHelper(void);

		~RobotHelper(void);

		Ptr<Robot> Install(Ptr<Node> node, Address address, uint16_t port, NodeContainer robots, RobotParameter parameters, map<uint32_t, RobotStatics> *stats);

	private:
		ObjectFactory factory;
	};

} // namespace ns3

#endif