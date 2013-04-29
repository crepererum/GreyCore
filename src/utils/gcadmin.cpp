#include "config.hpp"
#include "dbfile.hpp"
#include "dim.hpp"
#include "database.hpp"
#include "datatypes/dtstring.hpp"

#include <iostream>
#include <list>
#include <memory>

using namespace greycore;

std::list<std::string> readLine(std::istream& stream) {
	std::list<std::string> result;

	while (stream.peek() != '\n') {
		std::string s;
		stream >> s;
		result.push_back(s);
	}

	stream.ignore();

	return result;
}

int main() {
	// print hello message
	std::cout << "GreyCore" << std::endl;

	// setup
	std::cout << "Open DB: " << std::flush;
	Database db("db.bin");
	std::cout << "ok" << std::endl;

	// state
	bool exit = false;

	// create commands
	typedef const std::list<std::string>& cmdargs_t;
	std::map<std::string, std::function<void(cmdargs_t)>> commands;
	commands["createColumn"] = [&db](cmdargs_t args) {
		if (args.size() != 1) {
			std::cout << "Usage: createColumn COLUMN" << std::endl;
			return;
		}
		std::string colName = *args.begin();

		if (db.dimExists(colName)) {
			std::cout << "Nope, this dimension exists!" << std::endl;
		} else {
			db.createDim<int>(colName);
		}
	};
	commands["exit"] = [&exit](cmdargs_t){
		exit = true;
	};
	commands["listColumns"] = [&db](cmdargs_t){
		std::cout << *db.getIndexDims() << std::endl;
	};

	// main loop
	while (!exit) {
		// read new command
		std::string cmd;
		std::cout << ">> " << std::flush;
		auto cmdList = readLine(std::cin);

		// parse command
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

