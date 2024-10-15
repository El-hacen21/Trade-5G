#include "ns3/application-container.h"
#include "robot_helper.h"

using namespace std;
namespace ns3
{

	RobotHelper::RobotHelper(void)
	{
		factory.SetTypeId("ns3::Robot");
	}

	RobotHelper::~RobotHelper(void)
	{
	}

	Ptr<Robot> RobotHelper::Install(Ptr<Node> node, Address address, uint16_t port, NodeContainer robots, RobotParameter parameters, map<uint32_t, RobotStatics> *stats)
	{

		Ptr<Robot> app = factory.Create<Robot>();

		app->SetUp(address, port, robots, parameters, stats);
		node->AddApplication(app);

		return app;
	}

} // namespace ns3