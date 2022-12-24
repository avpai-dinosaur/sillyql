// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "TableEntry.h"
#include<vector>
#include<string>
#include<unordered_map>
#include<map>
#include<iostream>
#include<algorithm>

enum class Comparison : uint8_t
{
	Greater,
	Less,
	Equal
};

template <typename T>
class greaterFunctor {
public:
	greaterFunctor(T compValue_in, size_t colNumber_in) : compValue(compValue_in), colNumber(colNumber_in) {}

	bool operator()(std::vector<TableEntry> row) {
		return row[colNumber] > compValue;
	}

private:
	TableEntry compValue;
	size_t colNumber;
};

template <typename T>
class lessFunctor {
public:
	lessFunctor(T compValue_in, size_t colNumber_in) : compValue(compValue_in), colNumber(colNumber_in) {}

	bool operator()(std::vector<TableEntry> row) {
		return row[colNumber] < compValue;
	}

private:
	TableEntry compValue;
	size_t colNumber;
};

template <typename T>
class equalFunctor {
public:
	equalFunctor(T compValue_in, size_t colNumber_in) : compValue(compValue_in), colNumber(colNumber_in) {}

	bool operator()(std::vector<TableEntry> row) {
		return row[colNumber] == compValue;
	}

private:
	TableEntry compValue;
	size_t colNumber;
};

class Table {
public:
	Table() : hashIndexExists(false), bstIndexExists(false), numColumns(0), numRows(0) {}

	Table(std::string tableName, size_t tableDimension, std::vector<EntryType> colTypes, std::vector<std::string> colNames);

	void insertRow(size_t newRows);

	void printAll(std::vector<std::string> printCols, bool quietMode);

	void printWhereParse(std::string column, std::string comparison, std::vector<std::string> printCols, bool quietMode);

	template <typename T>
	void printWhere(std::string column, std::string comparison, std::vector<std::string> printCols, T value, bool quietMode);
	
	void deleteWhereParse(std::string column, std::string comparison);
	
	template <typename T>
	void deleteWhere(std::string column, std::string comparison, T value);

	void generateIndex(std::string indexType, std::string column);

	void joinWith(Table& table2, std::string col1, std::string col2, std::vector<std::pair<std::string, size_t>> printCols, bool quietMode);

private:
	std::string name;
	std::string indexCol;
	bool hashIndexExists;
	bool bstIndexExists;
	size_t numColumns;
	size_t numRows;
	std::map <std::string, std::pair<size_t, EntryType>> columnMap; // name -> (colNumber, dataType)
	std::vector<EntryType> columnVector; // colNumber -> (name, dataType)
	std::unordered_map<TableEntry, std::vector<size_t>> hashIndex; // value -> vector of rows
	std::map<TableEntry, std::vector<size_t>> bstIndex; // value -> vector of rows
	std::vector<std::vector<TableEntry>> data;
};
