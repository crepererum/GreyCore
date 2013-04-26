#include "config.hpp"
#include "dbfile.hpp"
#include "dim.hpp"
#include "datatypes/dtstring.hpp"

#include <iostream>
#include <list>
#include <memory>

std::list<std::string> splitCmd(const std::string& cmd) {
	std::list<std::string> result;
	std::size_t pos = 0;

	while (pos != std::string::npos) {
		auto next = cmd.find(" ", pos);
		auto itBegin = cmd.cbegin() + pos;
		auto itEnd = cmd.cend();
		if (next != std::string::npos) {
			itEnd = cmd.cbegin() + next;
		}

		if (next - pos > 1) {
			result.emplace_back(itBegin, itEnd);
		}

		pos = next;
	}

	return result;
}

int main() {
	// print hello message
	std::cout << "GreyCore" << std::endl;

	// setup
	std::cout << "Open DB: " << std::flush;
	auto db = std::make_shared<DBFile>("db.bin");
	std::cout << "ok" << std::endl;

	// state
	bool exit = false;
	auto columns = std::make_shared<Dim<DTString<colnameLength>>>(db, "_columns");

	// create commands
	typedef const std::list<std::string>& cmdargs_t;
	std::map<std::string, std::function<void(cmdargs_t)>> commands;
	commands["exit"] = [&exit](cmdargs_t){
		exit = true;
	};
	commands["listColumns"] = [&columns](cmdargs_t){
		for (std::size_t s = 0; s < columns->getSegmentCount(); ++s) {
			auto segment = columns->getSegment(s);
			for (std::size_t i = 0; i < columns->getSegmentFillSize(s); ++i) {
				std::cout << (*segment)[i] << std::endl;
			}
		}
	};

	// main loop
	while (!exit) {
		// read new command
		std::string cmd;
		std::cout << ">> " << std::flush;
		std::cin >> cmd;

		// parse command
		auto cmdList = splitCmd(cmd);
		if (cmdList.size() > 0) {
			auto cmdName = *cmdList.begin();
			decltype(cmdList) cmdArgs(++cmdList.cbegin(), cmdList.cend());
			try {
				commands.at(cmdName)(cmdArgs);
			} catch (const std::out_of_range& e) {
				std::cout << "unknown command \"" << cmdName << "\"" << std::endl;
			}
		}
	}

	// cleanup and exit
	return EXIT_SUCCESS;
}

