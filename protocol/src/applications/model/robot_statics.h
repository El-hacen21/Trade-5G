// #include "mempool.h"
#include "blockchain.h"

using namespace std;
using namespace ns3;

struct RobotStatics
{
    int expiredOffers = 0;
    int expiredDemands = 0;
};

struct RobotParameter
{
    int nb_robots = 0;     // Total number of transactions per tenant
    int seed = 0;          // Simulation seed
    int scenario_type = 0; // Scenario Type:
    int malicious = 0;
    int commission = 0;

    int INITIAL_RESOURCES_QUANTITY = 1000; //
    int INITIAL_RESOURCE_EXPIRATION_TIME = 30;
    double INITIAL_PRICE = 10;
    double MEAN_PRICE_OFFERS = 8;
    double MEAN_PRICE_DEMANDS = 10;
    float ALPHA = 0.5;                // Lifetime factor
    float BETA = 0.5;                 // Market Price factor
    float DELTA = 0.5;                // Market Price factor
    float EPSILON = 0.001;                // Epsilon
    double PRICE_STD_DEV = 0.198;     // The price
    double QUANTITY_LAMBDA = 5;       // The quantity of the
    double DURATION_LAMBDA_EXP = 0.1; // The time between transactions arrival rate
    double duration = 30 * 24 * 3600;
    double LIFE_TIME_LAMBDA = 0.1;

    double PRICE_AJUSTMENT_FACTOR = 10;
    double BASE_PRICE = 10;
};
