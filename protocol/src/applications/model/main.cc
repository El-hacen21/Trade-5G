/*
 * contains main
 * and code to compute some processes delay
 */

#include "experiment.h"

using namespace std;
using namespace ns3;

int main(int argc, char *argv[])
{

	if (argc == 7)
	{
		Experiment p(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

		cout << "./simulator " << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6] << " " << endl;

		p.Trace();
		p.Run();
	}
	else
		cout << "Arguments !!!" << endl;

	return 0;
}
