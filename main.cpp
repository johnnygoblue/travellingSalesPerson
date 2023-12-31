// Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include "SimpleGraph.h"
#include "MST.h"
#include "FASTTSP.h"
#include "OPTTSP.h"

#include <iostream>
#include <getopt.h>
//#include <cassert>
#include <cstring>
#include <iomanip>

using std::cout;
using std::cerr;
using std::cin;
using std::cerr;
using std::endl;

const bool BORDER_ON = true;
const bool BORDER_OFF = false;

enum class Mode { MST,
				FASTTSP,
				OPTTSP,
				ERR };

Mode getMode(int argc, char **argv) {
	int option_index = 0, option = 0;
	opterr = false;

	struct option longOpts[] = {{ "help", no_argument, nullptr, 'h'},
								{ "mode", required_argument, nullptr, 'm'},
								{ nullptr, 0, nullptr, '\0'}};

	while ((option = getopt_long(argc, argv, "m:h", longOpts, &option_index)) != -1) {
		switch (option) {
			case 'h':
				cout << "Gotta Catch 'Em All!\n";
				cout << "Supports three modes:\n";
				cout << "1) MST\n";
				cout << "2) FASTTSP\n";
				cout << "3) OPTTSP\n";
				exit(0);
			case 'm':
				if (strcmp(optarg, "MST\0") == 0) {
					return Mode::MST;
				} else if (strcmp(optarg, "FASTTSP\0") == 0) {
					return Mode::FASTTSP;
				} else if (strcmp(optarg, "OPTTSP\0") == 0) {
					return Mode::OPTTSP;
				} else {
					cerr << "Invalid input mode '" << optarg << "'\n";
					exit(1);
				}
			default:
				cerr << "Unrecognized option '" << option << "'" << endl;
				exit(1);
		} // switch
	} // while
	return Mode::ERR;
}

int main(int argc, char **argv) {
	/* setup output format and disable io sync */
	std::ios_base::sync_with_stdio(false);
	cout << std::setprecision(2);
	cout << std::fixed;

	Mode m = getMode(argc, argv);
	if (m == Mode::MST) {
		MST g (cin, BORDER_ON);
		g.gen_mst();
		g.print_mst(cout);
	}
	else if (m == Mode::FASTTSP) {
		FASTTSP g (cin, BORDER_OFF);
		g.arbitrary_insertion();
		g.print_tour(cout);
	}
	else if (m == Mode::OPTTSP) {
		OPTTSP g (cin, BORDER_OFF);
		g.compute_tour();
		g.print_tour(cout);
	}
	else if (m ==  Mode::ERR) {
		cerr << "Error mode caught!" << endl;
	}
	else {
		cerr << "Program failed to execute due to invalid mode caught!" << endl;
	}
	return 0;
}
