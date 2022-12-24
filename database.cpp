// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include"database.h"

// Parses command line options
void Database::get_options(int argc, char** argv) {
	int option_index = 0, option = 0;

	// Don't display getopt error messages about options
	opterr = false;

	// use getopt to find command line options
	struct option longOpts[] = {
		{"quiet", no_argument, nullptr, 'q'},
		{"help", no_argument, nullptr, 'h'},
		{ nullptr, 0, nullptr, '\0' } };

	while ((option = getopt_long(argc, argv, "qh", longOpts, &option_index)) != -1) {
		switch (option) {
		case 'q':
			quietMode = true;
			break;
		case 'h':
			std::cout << "SillyQL help message" << std::endl;
			exit(0);
			break;
		default:
			std::cerr << "Unknown command line option" << std::endl;
			exit(1);
		}
	}
}

void Database::createTable() {
	std::string tableName;
	std::cin >> tableName;

	if (tables.find(tableName) != tables.end()) {
		std::cout << "Error during CREATE: ";
		std::cout << "Cannot create already existing table " << tableName << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	int tableDimension;
	std::cin >> tableDimension;

	std::vector<EntryType> colTypes;
	for (int i = 0; i < tableDimension; ++i) {
		std::string colType;
		std::cin >> colType;
		if (colType == "double") {
			colTypes.push_back(EntryType::Double);
		}
		else if (colType == "bool") {
			colTypes.push_back(EntryType::Bool);
		}
		else if (colType == "int") {
			colTypes.push_back(EntryType::Int);
		}
		else if (colType == "string") {
			colTypes.push_back(EntryType::String);
		}
	}

	std::vector<std::string> colNames;
	for (int i = 0; i < tableDimension; ++i) {
		std::string colName;
		std::cin >> colName;
		colNames.push_back(colName);
	}

	tables[tableName] = Table(tableName, tableDimension, colTypes, colNames);

	std::cout << "New table " << tableName << " with column(s) ";
	for (size_t i = 0; i < colNames.size(); ++i) {
		std::cout << colNames[i] << " ";
	}
	std::cout << "created" << "\n";
}

void Database::removeTable() {
	std::string tableName;
	std::cin >> tableName;

	if (tables.find(tableName) == tables.end()) {
		std::cout << "Error during REMOVE: ";
		std::cout << tableName << " does not name a table in the database" << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	tables.erase(tableName);
	std::cout << "Table " << tableName << " deleted" << "\n";
}

void Database::insert() {
	std::string junk;
	std::string tableName;
	size_t numRows;

	std::cin >> junk >> tableName >> numRows >> junk;

	if (tables.find(tableName) == tables.end()) {
		std::cout << "Error during INSERT: ";
		std::cout << tableName << " does not name a table in the database" << "\n";
		getline(std::cin, junk);
		return;
	}

	/*if (numRows == 0) {
		std::cout << "Error during INSERT: ";
		std::cout << "invalid command" << "\n";
		getline(std::cin, junk);
		return;
	}*/

	tables[tableName].insertRow(numRows);
}

void Database::print() {
	std::string junk;
	std::string tableName;
	size_t numCols;
	std::vector<std::string> printCols;
	std::string WHERE_ALL;

	std::cin >> junk >> tableName >> numCols;

	if (tables.find(tableName) == tables.end()) {
		std::cout << "Error during PRINT: ";
		std::cout << tableName << " does not name a table in the database" << "\n";
		getline(std::cin, junk);
		return;
	}

	printCols.reserve(numCols);
	for (size_t i = 0; i < numCols; ++i) {
		std::string printCol;
		std::cin >> printCol;
		printCols.push_back(printCol);
	}

	std::cin >> WHERE_ALL;
	if (WHERE_ALL == "ALL") {
		tables[tableName].printAll(printCols, quietMode);
	}
	else {
		std::string column;
		std::string comparison;
		std::cin >> column >> comparison;
		tables[tableName].printWhereParse(column, comparison, printCols, quietMode);
	}
}

void Database::deleteFromTable() {
	std::string junk;
	std::string tableName;
	std::string column;
	std::string comparison;

	std::cin >> junk >> tableName >> junk >> column >> comparison;

	if (tables.find(tableName) == tables.end()) {
		std::cout << "Error during DELETE: ";
		std::cout << tableName << " does not name a table in the database" << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	tables[tableName].deleteWhereParse(column, comparison);
}

void Database::generate() {
	std::string junk;
	std::string tableName;
	std::string indexType; // bst or hash
	std::string column;

	std::cin >> junk >> tableName >> indexType >> junk >> junk >> column;

	if (tables.find(tableName) == tables.end()) {
		std::cout << "Error during GENERATE: ";
		std::cout << tableName << " does not name a table in the database" << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	tables[tableName].generateIndex(indexType, column);
}

void Database::join() {
	std::string table1;
	std::string junk;
	std::string table2;
	std::string col1;
	std::string col2;
	size_t numColumns;
	std::vector<std::pair<std::string, size_t>> printCols;

	std::cin >> table1 >> junk >> table2 >> junk >> col1 >> junk >> col2 >> junk >> junk >> numColumns;

	if (tables.find(table1) == tables.end()) {
		std::cout << "Error during JOIN: ";
		std::cout << table1 << " does not name a table in the database" << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	if (tables.find(table2) == tables.end()) {
		std::cout << "Error during JOIN: ";
		std::cout << table2 << " does not name a table in the database" << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	printCols.reserve(numColumns);
	for (size_t i = 0; i < numColumns; ++i) {
		size_t oneTwo;
		std::string column;
		std::cin >> column >> oneTwo;
		printCols.push_back({ column, oneTwo });
	}

	tables[table1].joinWith(tables[table2], col1, col2, printCols, quietMode);
}

void Database::runDatabase() {
	std::string cmd;
	do {
		if (std::cin.fail()) {
			std::cerr << "Error: Reading from cin has failed" << std::endl;
			exit(1);
		}

		std::cout << "% ";
		std::cin >> cmd;
		//TODO - process cmd

		/*if (cmd[0] != ('#' || 'C' || 'R' || 'I' || 'P' || 'D' || 'G' || 'J' || 'Q')) {
			std::cout << "Error: unrecognized command" << "\n";
			std::string junk;
			getline(std::cin, junk);
		}*/

		if (cmd[0] == '#') {
			std::string junk;
			std::getline(std::cin, junk);
			continue;
		}
		else if (cmd == "CREATE") {
			createTable();
		}
		else if (cmd == "REMOVE") {
			removeTable();
		}
		else if (cmd == "INSERT") {
			insert();
		}
		else if (cmd == "PRINT") {
			print();
		}
		else if (cmd == "DELETE") {
			deleteFromTable();
		}
		else if (cmd == "GENERATE") {
			generate();
		}
		else if (cmd == "JOIN") {
			join();
		}
		else if (cmd != "QUIT") {
			std::cout << "Error: unrecognized command" << "\n";
			std::string junk;
			getline(std::cin, junk);
		}
	} while (cmd != "QUIT");

	std::cout << "Thanks for being silly!" << "\n";
}