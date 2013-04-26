#include <iostream>

int main() {
	// print hello message
	std::cout << "GreyCore" << std::endl;

	// main loop
	bool exit = false;
	while (!exit) {
		// read new command
		std::string cmd;
		std::cout << ">> " << std::flush;
		std::cin >> cmd;

		// parse command
		if (cmd == "exit") {
			exit = true;
		}
	}

	// cleanup and exit
	return EXIT_SUCCESS;
}

