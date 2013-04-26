#include "dbfile.hpp"
#include "dim.hpp"

#include <iostream>
#include <memory>

int main() {
	// print hello message
	std::cout << "GreyCore" << std::endl;

	// setup
	std::cout << "Open DB: " << std::flush;
	auto db = std::make_shared<DBFile>("db.bin");
	std::cout << "ok" << std::endl;

	// main loop
	bool exit = false;
	auto dim = std::make_shared<Dim<int>>(db, "test");
	while (!exit) {
		// read new command
		std::string cmd;
		std::cout << ">> " << std::flush;
		std::cin >> cmd;

		// parse command
		if (cmd == "exit") {
			exit = true;
		} else if (cmd == "add") {
			dim->add(0);
		} else if (cmd == "show") {
			for (int s = 0; s < dim->getSegmentCount(); ++s) {
				auto segment = dim->getSegment(s);
				for (int i = 0; i < dim->getSegmentFillSize(s); ++i) {
					std::cout << (*segment)[i] << std::endl;
				}
			}
		}
	}

	// cleanup and exit
	return EXIT_SUCCESS;
}

