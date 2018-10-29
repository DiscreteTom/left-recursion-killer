#pragma once

#include <string>
#include <vector>

using namespace std;

class SymbolTable
{
private:
	vector<string> strs;

public:
	int getIndex(const string &str);
	string getStr(int i);
};

struct Symbol
{
	enum SymbolType
	{
		T, // terminator
		NT // non-terminator
	};
	SymbolType type;
	int c; // symbol index or char
}