// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "table.h"

Table::Table(std::string tableName, size_t tableDimension, std::vector<EntryType> colTypes, std::vector<std::string> colNames) {
	name = tableName;
	hashIndexExists = false;
	bstIndexExists = false;
	numColumns = tableDimension;
	numRows = 0;
	for (size_t i = 0; i < tableDimension; ++i) {
		columnMap[colNames[i]] = { i, colTypes[i] };
		columnVector.push_back(colTypes[i]);
	}
}

void Table::insertRow(size_t newRows) {
	data.resize(numRows + newRows);
	for (size_t i = numRows; i < numRows + newRows; ++i) {
		data[i].reserve(numColumns);
	}


	for (size_t i = numRows; i < numRows + newRows; ++i) {
		for (size_t j = 0; j < numColumns; ++j) {
			switch (columnVector[j])
			{
			case EntryType::Bool:
				bool input1;
				std::cin >> input1;
				data[i].emplace_back(input1);
				break;

			case EntryType::Double:
				double input2;
				std::cin >> input2;
				data[i].emplace_back(input2);
				break;

			case EntryType::Int:
				int input3;
				std::cin >> input3;
				data[i].emplace_back(input3);
				break;

			case EntryType::String:
				std::string input4;
				std::cin >> input4;
				data[i].emplace_back(input4);
				break;
			}
		}
	}

	// add to indices if they exist
	if (hashIndexExists || bstIndexExists) {
		if (hashIndexExists) {
			for (auto val : hashIndex) {
				val.second.reserve(numRows + newRows);
			}

			for (size_t i = numRows; i < numRows + newRows; ++i) {
				hashIndex[data[i][columnMap[indexCol].first]].push_back(i);
			}
		}
		else { // bstIndexExists
			for (auto val : bstIndex) {
				val.second.reserve(numRows + newRows);
			}

			for (size_t i = numRows; i < numRows + newRows; ++i) {
				bstIndex[data[i][columnMap[indexCol].first]].push_back(i);
			}
		}
	}

	std::cout << "Added " << newRows << " rows to " << name << " from position " << numRows << " to " << numRows + newRows - 1 << "\n";

	numRows += newRows;
}

void Table::printAll(std::vector<std::string> printCols, bool quietMode) {
	for (auto col : printCols) {
		if (columnMap.find(col) == columnMap.end()) {
			std::cout << "Error during PRINT: ";
			std::cout << col << " does not name a column in " << name << "\n";
			std::string junk;
			getline(std::cin, junk);
			return;
		}
	}

	for (auto col : printCols) {
		if (!quietMode) {
			std::cout << col << " ";
		}
	}
	if (!quietMode) {
		std::cout << "\n";
	}

	if (!quietMode) {
		for (size_t i = 0; i < numRows; ++i) {
			for (auto col : printCols) {
				std::cout << data[i][columnMap[col].first] << " ";
			}
			std::cout << "\n";
		}
	}

	std::cout << "Printed " << numRows << " matching rows from " << name << "\n";
}

void Table::printWhereParse(std::string column, std::string comparison, std::vector<std::string> printCols, bool quietMode) {
	switch (columnMap[column].second)
	{
	case EntryType::Bool:
		bool value1;
		std::cin >> value1;
		printWhere<bool>(column, comparison, printCols, value1, quietMode);
		break;
	case EntryType::Double:
		double value2;
		std::cin >> value2;
		printWhere<double>(column, comparison, printCols, value2, quietMode);
		break;
	case EntryType::Int:
		int value3;
		std::cin >> value3;
		printWhere<int>(column, comparison, printCols, value3, quietMode);
		break;
	case EntryType::String:
		std::string value4;
		std::cin >> value4;
		printWhere<std::string>(column, comparison, printCols, value4, quietMode);
		break;
	}
}

template <typename T>
void Table::printWhere(std::string column, std::string comparison, std::vector<std::string> printCols, T value, bool quietMode) {
	for (auto col : printCols) {
		if (columnMap.find(col) == columnMap.end()) {
			std::cout << "Error during PRINT: ";
			std::cout << col << " does not name a column in " << name << "\n";
			std::string junk;
			getline(std::cin, junk);
			return;
		}
	}

	for (auto col : printCols) {
		if (!quietMode) {
			std::cout << col << " ";
		}
	}
	if (!quietMode) {
		std::cout << "\n";
	}

	if (comparison == "=") {
		size_t numRowsPrinted = 0;
		if (hashIndexExists && (column == indexCol)) {
			TableEntry entry(value);
			for (size_t i = 0; i < hashIndex[entry].size(); ++i) {
				if (!quietMode) {
					for (auto col : printCols) {
						std::cout << data[hashIndex[entry][i]][columnMap[col].first] << " ";
					}
					std::cout << "\n";
				}
				++numRowsPrinted;
			}
		}
		else if (bstIndexExists && (column == indexCol)) {
			TableEntry entry(value);
			for (size_t i = 0; i < bstIndex[entry].size(); ++i) {
				if (!quietMode) {
					for (auto col : printCols) {
						std::cout << data[bstIndex[entry][i]][columnMap[col].first] << " ";
					}
					std::cout << "\n";
				}
				++numRowsPrinted;
			}
		}
		else {
			equalFunctor<T> equal(value, columnMap[column].first);
			for (size_t i = 0; i < numRows; ++i) {
				if (equal(data[i])) {
					if (!quietMode) {
						for (auto col : printCols) {
							std::cout << data[i][columnMap[col].first] << " ";
						}
						std::cout << "\n";
					}
					++numRowsPrinted;
				}
			}
		}
		std::cout << "Printed " << numRowsPrinted << " matching rows from " << name << "\n";
		return;
	}
	else if (comparison == ">") {
		size_t numRowsPrinted = 0;
		if (bstIndexExists && (column == indexCol)) {
			TableEntry entry(value);
			for (auto it = bstIndex.upper_bound(entry); it != bstIndex.end(); ++it) {
				for (size_t i = 0; i < it->second.size(); ++i) {
					if (!quietMode) {
						for (auto col : printCols) {
							std::cout << data[it->second[i]][columnMap[col].first] << " ";
						}
						std::cout << "\n";
					}
					++numRowsPrinted;
				}
			}
		}
		else {
			greaterFunctor<T> greater(value, columnMap[column].first);
			for (size_t i = 0; i < numRows; ++i) {
				if (greater(data[i])) {
					if (!quietMode) {
						for (auto col : printCols) {
							std::cout << data[i][columnMap[col].first] << " ";
						}
						std::cout << "\n";
					}
					++numRowsPrinted;
				}
			}
		}
		std::cout << "Printed " << numRowsPrinted << " matching rows from " << name << "\n";
		return;
	}
	else {
		size_t numRowsPrinted = 0;
		if (bstIndexExists && (column == indexCol)) {
			TableEntry entry(value);
			auto it = bstIndex.lower_bound(entry);
			for (auto ptr = bstIndex.begin(); ptr != it; ++ptr) {
				for (size_t i = 0; i < ptr->second.size(); ++i) {
					if (!quietMode) {
						for (auto col : printCols) {
							std::cout << data[ptr->second[i]][columnMap[col].first] << " ";
						}
						std::cout << "\n";
					}
					++numRowsPrinted;
				}
			}
		}
		else {
			lessFunctor<T> less(value, columnMap[column].first);
			for (size_t i = 0; i < numRows; ++i) {
				if (less(data[i])) {
					if (!quietMode) {
						for (auto col : printCols) {
							std::cout << data[i][columnMap[col].first] << " ";
						}
						std::cout << "\n";
					}
					++numRowsPrinted;
				}
			}
		}
		std::cout << "Printed " << numRowsPrinted << " matching rows from " << name << "\n";
		return;
	}
}

void Table::deleteWhereParse(std::string column, std::string comparison) {
	if (columnMap.find(column) == columnMap.end()) {
		std::cout << "Error during DELETE: ";
		std::cout << column << " does not name a column in " << name << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	switch (columnMap[column].second)
	{
	case EntryType::Bool:
		bool value1;
		std::cin >> value1;
		deleteWhere<bool>(column, comparison, value1);
		break;
	case EntryType::Double:
		double value2;
		std::cin >> value2;
		deleteWhere<double>(column, comparison, value2);
		break;
	case EntryType::Int:
		int value3;
		std::cin >> value3;
		deleteWhere<int>(column, comparison, value3);
		break;
	case EntryType::String:
		std::string value4;
		std::cin >> value4;
		deleteWhere<std::string>(column, comparison, value4);
		break;
	}
}

template <typename T>
void Table::deleteWhere(std::string column, std::string comparison, T value) {
	if (comparison == "=") {
		equalFunctor<T> equal(value, columnMap[column].first);
		auto it = std::remove_if(data.begin(), data.end(), equal);
		size_t newRows = 0;
		for (auto ptr = data.begin(); ptr != it; ++ptr) {
			++newRows;
		}
		data.resize(newRows);

		std::cout << "Deleted " << numRows - newRows << " rows from " << name << "\n";

		numRows = newRows;
	}
	else if (comparison == ">") {
		greaterFunctor<T> greater(value, columnMap[column].first);
		auto it = std::remove_if(data.begin(), data.end(), greater);
		size_t newRows = 0;
		for (auto ptr = data.begin(); ptr != it; ++ptr) {
			++newRows;
		}
		data.resize(newRows);

		std::cout << "Deleted " << numRows - newRows << " rows from " << name << "\n";

		numRows = newRows;
	}
	else {
		lessFunctor<T> less(value, columnMap[column].first);
		auto it = std::remove_if(data.begin(), data.end(), less);
		size_t newRows = 0;
		for (auto ptr = data.begin(); ptr != it; ++ptr) {
			++newRows;
		}
		data.resize(newRows);

		std::cout << "Deleted " << numRows - newRows << " rows from " << name << "\n";

		numRows = newRows;
	}

	// re-generate indices if they exist
	if (hashIndexExists || bstIndexExists) {
		if (hashIndexExists) {
			hashIndex.clear();

			for (auto val : hashIndex) {
				val.second.reserve(numRows);
			}

			for (size_t i = 0; i < numRows; ++i) {
				hashIndex[data[i][columnMap[indexCol].first]].push_back(i);
			}
		}
		else { // bstIndexExists
			bstIndex.clear();

			for (auto val : bstIndex) {
				val.second.reserve(numRows);
			}

			for (size_t i = 0; i < numRows; ++i) {
				bstIndex[data[i][columnMap[indexCol].first]].push_back(i);
			}
		}
	}
}

void Table::generateIndex(std::string indexType, std::string column) {
	if (columnMap.find(column) == columnMap.end()) {
		std::cout << "Error during GENERATE: ";
		std::cout << column << " does not name a column in " << name << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	if (indexType == "bst") {
		hashIndex.clear();
		bstIndex.clear();

		hashIndexExists = false;
		bstIndexExists = true;

		for (auto val : hashIndex) {
			val.second.reserve(numRows);
		}

		for (size_t i = 0; i < numRows; ++i) {
			bstIndex[data[i][columnMap[column].first]].push_back(i);
		}
	}
	else { // hash
		hashIndex.clear();
		bstIndex.clear();

		hashIndexExists = true;
		bstIndexExists = false;

		for (auto val : bstIndex) {
			val.second.reserve(numRows);
		}

		for (size_t i = 0; i < numRows; ++i) {
			hashIndex[data[i][columnMap[column].first]].push_back(i);
		}
	}

	indexCol = column;
	std::cout << "Created " << indexType << " index for table " << name << " on column " << column << "\n";
}

void Table::joinWith(Table& table2, std::string col1, std::string col2, std::vector<std::pair<std::string, size_t>> printCols, bool quietMode) {
	if (columnMap.find(col1) == columnMap.end()) {
		std::cout << "Error during JOIN: ";
		std::cout << col1 << " does not name a column in " << name << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}

	if (table2.columnMap.find(col2) == table2.columnMap.end()) {
		std::cout << "Error during JOIN: ";
		std::cout << col2 << " does not name a column in " << table2.name << "\n";
		std::string junk;
		getline(std::cin, junk);
		return;
	}


	for (auto k : printCols) {
		if (k.second == 1) {
			if (columnMap.find(k.first) == columnMap.end()) {
				std::cout << "Error during JOIN: ";
				std::cout << k.first << " does not name a column in " << name << "\n";
				std::string junk;
				getline(std::cin, junk);
				return;
			}
		}
		else {
			if (table2.columnMap.find(k.first) == table2.columnMap.end()) {
				std::cout << "Error during JOIN: ";
				std::cout << k.first << " does not name a column in " << table2.name << "\n";
				std::string junk;
				getline(std::cin, junk);
				return;
			}
		}
	}

	for (auto k : printCols) {
		if (!quietMode) {
			std::cout << k.first << " ";
		}
	}
	if (!quietMode) {
		std::cout << "\n";
	}

	size_t numRowsPrinted = 0;
	if (table2.hashIndexExists && (table2.indexCol == col2)) {
		for (size_t i = 0; i < numRows; ++i) {
			for (auto j : table2.hashIndex[data[i][columnMap[col1].first]]) {
				if (!quietMode) {
					for (auto k : printCols) {
						if (k.second == 1) {
							std::cout << data[i][columnMap[k.first].first] << " ";
						}
						else {
							std::cout << table2.data[j][table2.columnMap[k.first].first] << " ";
						}
					}
					std::cout << "\n";
				}
				++numRowsPrinted;
			}
		}
	}
	else if (table2.bstIndexExists && (table2.indexCol == col2)) {
		for (size_t i = 0; i < numRows; ++i) {
			for (auto j : table2.bstIndex[data[i][columnMap[col1].first]]) {
				if (!quietMode) {
					for (auto k : printCols) {
						if (k.second == 1) {
							std::cout << data[i][columnMap[k.first].first] << " ";
						}
						else {
							std::cout << table2.data[j][table2.columnMap[k.first].first] << " ";
						}
					}
					std::cout << "\n";
				}
				++numRowsPrinted;
			}
		}
	}
	else { // means no index
		std::unordered_map<TableEntry, std::vector<size_t>> tempHashIndexT2;

		for (auto val : tempHashIndexT2) {
			val.second.reserve(table2.numRows);
		}

		for (size_t i = 0; i < table2.numRows; ++i) {
			tempHashIndexT2[table2.data[i][table2.columnMap[col2].first]].push_back(i);
		}

		for (size_t i = 0; i < numRows; ++i) {
			for (auto j : tempHashIndexT2[data[i][columnMap[col1].first]]) {
				if (!quietMode) {
					for (auto k : printCols) {
						if (k.second == 1) {
							std::cout << data[i][columnMap[k.first].first] << " ";
						}
						else {
							std::cout << table2.data[j][table2.columnMap[k.first].first] << " ";
						}
					}
					std::cout << "\n";
				}
				++numRowsPrinted;
			}
		}
	}

	std::cout << "Printed " << numRowsPrinted << " rows from joining " << name << " to " << table2.name << "\n";
}
