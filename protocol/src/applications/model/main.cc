#include "experiment.h"
#include <map>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace ns3;

int main(int argc, char *argv[])
{
    // Map to store parameters and their default values
    map<string, string> params = {
        {"nb_robots", "4"},
        {"scenario", "0"},
        {"seed", "42"},
        {"malicious", "0"},
        {"commission", "5"},
        {"duration", "100"}
    };

    // Parse arguments in --parameter=value format
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        size_t eq_pos = arg.find('=');
        if (eq_pos != string::npos && arg.substr(0, 2) == "--")
        {
            string key = arg.substr(2, eq_pos - 2); // Remove "--" and extract key
            string value = arg.substr(eq_pos + 1);  // Extract value
            if (params.find(key) != params.end())
            {
                params[key] = value; // Update the parameter value
            }
            else
            {
                cerr << "Unknown parameter: " << key << endl;
                return 1;
            }
        }
        else
        {
            cerr << "Invalid argument format: " << arg << endl;
            return 1;
        }
    }

    // Convert parameters to integers for the Experiment constructor
    int nb_robots = stoi(params["nb_robots"]);
    int scenario = stoi(params["scenario"]);
    int seed = stoi(params["seed"]);
    int malicious = stoi(params["malicious"]);
    int commission = stoi(params["commission"]);
    int duration = stoi(params["duration"]);

    // Print parsed parameters for confirmation
    cout << "Running simulator with the following parameters:" << endl;
    for (const auto &param : params)
    {
        cout << "--" << param.first << "=" << param.second << endl;
    }

    // Run the simulation
    Experiment p(nb_robots, scenario, seed, malicious, commission, duration);

    p.Trace();
    p.Run();

    return 0;
}
