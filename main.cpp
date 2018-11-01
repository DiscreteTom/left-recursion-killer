#include <iostream>
#include <string>
#include "gramma.h"

using namespace std;

int main(int argc, char ** argv){
	if (argc == 1){
		//no file input
		GrammaTable gt;
		string str;
		getline(cin, str);
		while (str.length()){
			gt.insert(str);
			getline(cin, str);
		}
		gt.start();
	} else {
		GrammaTable gt(argv[1]);
	}
	system("pause");
}