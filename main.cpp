#include <iostream>
#include <string>
#include "gramma.h"

using namespace std;

void showDeveloper();
void showHelp();

int main(int argc, char **argv)
{
	showDeveloper();
	if (argc == 1)
	{
		showHelp();
		//no file input
		GrammaTable gt;
		string str;
		getline(cin, str);
		while (str.length())
		{
			gt.insert(str);
			getline(cin, str);
		}
		try
		{
			gt.start();
		}
		catch (string err)
		{
			cout << err;
		}
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			cout << "============================ " << argv[i] << "==============================\n";
			try
			{
				GrammaTable gt(argv[i]);
			}
			catch (string str)
			{
				cout << str;
			}
		}
	}
	system("pause");
}

void showDeveloper()
{
	cout << "*************************************************************\n"
			 << "                  Left-Recursion-Killer\n"
			 << "                 Written By DiscreteTom\n"
			 << "             See source code and report BUG at\n"
			 << "    https://github.com/DiscreteTom/left-recursion-killer\n"
			 << "*************************************************************\n\n";
}

void showHelp()
{
	cout << "Drag file(s) on this exe to run.\n"
			 << "Or run this exe then input.\n"
			 << "Format: A -> xxx | xxx\n"
			 << "Input '~' for epsilon\n"
			 << "Input an empty line for end\n\n";
}