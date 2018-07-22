#include<iostream>
#include "Computing.h"
#include <fstream>

using namespace std;
 
int main()
{
	int inputArr[12];
	ifstream inputFile("input.txt");
	if(inputFile.is_open()){	
		for(int i=0; i<12; i++){
			inputFile >> inputArr[i];
		}
	}
	inputFile.close();

	Computing comp(3, 0);
	comp.distribute(inputArr, 12);

	cout << comp.access(10) << endl;

    return 0;
}
