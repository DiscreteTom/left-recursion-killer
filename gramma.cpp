#include "gramma.h"

#include <fstream>
#include <iostream>

using namespace std;

GrammaTable::GrammaTable(const string &fileName)
{
	tTable.getIndex("~"); // add epsilon to terminator table

	if (fileName.length())
	{
		loadFromFile(fileName);
		start();
	}
}

void GrammaTable::loadFromFile(const string &fileName)
{
	string str;
	fstream fin;
	fin.open(fileName);
	while (fin.good())
	{
		getline(fin, str);
		insert(str);
	}
	fin.close();
}

string GrammaTable::format(const string &str)
{
	string result;
	for (auto c : str)
	{
		if (c != ' ' && c != '\t')
			result += c;
	}
	//check format
	int i = str.find('-');
	if (str[i + 1] != '>')
		result = "";
	return result;
}

void GrammaTable::killExplicit(int index)
{
	vector<vector<Symbol>> newCandidates;
	for (int i = 0; i < grammas[index].size(); ++i) // for each candidate
	{
		if (grammas[index][i][0].type == Symbol::SymbolType::NT && grammas[index][i][0].index == index)
		{
			//explicit left recursion exist
			//get left recursion candidate and delete the origin one
			vector<Symbol> candidate = grammas[index][i];
			grammas[index].erase(grammas[index].begin() + i);
			newCandidates.push_back(candidate);
		}
	}

	if (newCandidates.size()) // explicit left recursion exist
	{
		//add a new NT
		int newIndex = ntTable.getIndex(ntTable.getStr(index) + '\'');
		//construct this new NT
		for (int i = 0; i < newCandidates.size(); ++i)
		{
			//erase first element, kill left recursion
			newCandidates[i].erase(newCandidates[i].begin());
			//add new symbol to the end
			newCandidates[i].push_back({Symbol::SymbolType::NT, newIndex});
		}
		// //add epsilon
		// vector<Symbol> epsilon;
		// epsilon.push_back({Symbol::SymbolType::T, 0}); // 0 for epsilon
		// newCandidates.push_back(epsilon);
		grammas.push_back(newCandidates);

		// renew old candidate
		for (int i = 0; i < grammas[index].size(); ++i)
		{
			grammas[index][i].push_back({Symbol::SymbolType::NT, newIndex});
		}
		if (!grammas[index].size()){
			vector<Symbol> t;
			t.push_back({Symbol::SymbolType::NT, newIndex});
			grammas[index].push_back(t);
		}
	}
}

void GrammaTable::insert(const string &grammaLine)
{
	string str = format(grammaLine);
	if (!str.length()) return;// ERROR

	// get left Symbol string
	string left;
	left += str[0];
	int i = 1; // index of str
	while (i < str.length() && str[i] == '\'')
	{
		left += str[i];
		++i;
	}

	// check left Symbol
	int grammaIndex = ntTable.getIndex(left);
	if (grammaIndex == grammas.size()) //new symbol
	{
		grammas.push_back(vector<vector<Symbol>>());
	}

	// get right
	i += 2; // read "->"
	vector<Symbol> singleRight;
	string sym; // current symbol string
	while (i < str.length())
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			sym += str[i];
			while (i + 1 < str.length() && str[i + 1] == '\'')
			{
				++i;
				sym += str[i];
			}
			//find this NT
			int index = ntTable.getIndex(sym);
			if (index == grammas.size())// new NT
			{
				grammas.push_back(vector<vector<Symbol>>());
			}
			singleRight.push_back({Symbol::SymbolType::NT, index});
			sym = "";
		}
		else if (str[i] == '|')
		{
			if (singleRight.size())
			{
				grammas[grammaIndex].push_back(singleRight);
				singleRight.clear();
			}
		}
		else
		{
			//other characters, inlcude '~', see them as terminator
			sym = str[i];
			while (i + 1 < str.length() && str[i + 1] == '\''){
				sym += str[i + 1];
				++i;
			}
			int index = tTable.getIndex(sym);
			singleRight.push_back({Symbol::SymbolType::T, index});
			sym = "";
		}
		++i;
	}
	if (singleRight.size())
	{
		grammas[grammaIndex].push_back(singleRight);
	}
}

void GrammaTable::start()
{
	// if (!isOK())
	// {
	// 	cout << "Error input!\n";
	// 	return;
	// }

	for (int i = 0; i < grammas.size(); ++i)
	{
		//eliminate explicit left recursion
		killExplicit(i);
		for (int j = 0; j < i; ++j)
		{
			// find Ai -> Ajxxx
			for (int k = 0; k < grammas[i].size(); ++k)
			{
				//for each candidate
				if (grammas[i][k][0].type == Symbol::SymbolType::NT && grammas[i][k][0].index == j)
				{
					// grammas[i][k]: Ai -> Ajxxx
					auto AjCandidates = grammas[j];
					for (auto candidate : AjCandidates)
					{
						candidate.insert(candidate.end(), grammas[i][k].begin() + 1, grammas[i][k].end());
						grammas[i].push_back(candidate);
					}
					grammas[i].erase(grammas[i].begin() + k);
					killExplicit(i);
				}
			}
		}
		// killUseless();
	}
	output();
}

void GrammaTable::output()
{
	for (int i = 0; i < grammas.size(); ++i)
	{
		cout << ntTable.getStr(i) << " -> ";
		for (int j = 0; j < grammas[i].size(); ++j)
		{
			//each candidate
			for (int k = 0; k < grammas[i][j].size(); ++k)
			{
				if (grammas[i][j][k].type == Symbol::SymbolType::NT)
				{
					cout << ntTable.getStr(grammas[i][j][k].index);
				}
				else // type == T
				{
					cout << tTable.getStr(grammas[i][j][k].index);
				}
			}
			if (j != grammas[i].size() - 1)
				cout << ' | ';
		}
		cout << endl;
	}
	cout << endl;
}
