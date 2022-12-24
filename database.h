// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "table.h"
#include <getopt.h>
#include<unordered_map>
#include<iostream>

class Database {
public:
	Database() : quietMode(false) {}

	// Parses command line options
	void get_options(int argc, char** argv);

	void runDatabase();

private:
	std::unordered_map<std::string, Table> tables;
	bool quietMode;

	void createTable();

	void removeTable();

	void insert();

	void print();

	void deleteFromTable();

	void generate();

	void join();
};
