#pragma once

#include <string>
#include <vector>

using namespace std;

/***************************************
 * 
 * Non-terminator - Capital characters
 * Arrow          - "->"
 * Divider        - '|'
 * Epsilon        - '~'
 * Terminator     - Other visible characters
 * 
 *************************************/

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