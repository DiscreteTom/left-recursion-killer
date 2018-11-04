#pragma once

#include <string>
#include <vector>
#include "symbol.h"

using namespace std;

class GrammaTable
{
private:
	vector<vector<vector<Symbol>>> grammas; // NT - Candidate - Symbol
	SymbolTable tTable;											// terminator table
	SymbolTable ntTable;										// non-terminator table

	void loadFromFile(const string &fileName);
	string format(const string &str); // discard blank characters
	void killExplicit(int index);
	void killEpsilon();
	// void killUseless();

public:
	GrammaTable(const string &fileName = "");
	void insert(const string &grammaLine); // format: NT -> xxx | xxx | ...
	void start();
	void output();
};