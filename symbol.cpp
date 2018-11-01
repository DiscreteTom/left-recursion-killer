#include "symbol.h"

int SymbolTable::getIndex(const string &str)
{
	for (int i = 0; i < strs.size(); ++i)
	{
		if (strs[i] == str) // str exist
			return i;
	}
	// str does NOT exist, add it to table
	strs.push_back(str);
	return strs.size() - 1;
}

string SymbolTable::getStr(int i)
{
	if (i >= 0 && i < strs.size())
		return strs[i];
	else
		throw "SymbolTable::getStr overflow!";
}