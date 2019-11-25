#include <getopt.h>
#include <cassert>
#include <string>
#include <iomanip>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;


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

	switch (getMode(argc, argv)) {
		case Mode::MST:
			cout << "MST mode on!\n";
			break;
		case Mode::FASTTSP:
			cout << "FASTTSP mode on!\n";
			break;
		case Mode::OPTTSP:
			cout << "OPTTSP mode on!\n";
			break;
		case Mode::ERR:
			cerr << "ERR mode caught" << endl;
			break;
		default:
			cerr << "Program failed to execute due to invalid mode received" << endl;
			break;
	} // switch
	return 0;
}
