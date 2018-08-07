#include<iostream>
#include "Computing.h"
#include <fstream>

using namespace std;

void sumMatrices(Computing &c, int row, int col); 

int main()
{
	int inputArr[24];
	ifstream inputFile("input.txt");
	if(inputFile.is_open()){	
		for(int i=0; i<24; i++){
			inputFile >> inputArr[i];
		}
	}
	inputFile.close();

	Computing comp(3, 0);
	comp.distribute(inputArr, 24);
	sumMatrices(comp, 3, 4);

    return 0;
}

void sumMatrices(Computing &c, int row, int col){
	int mat[row][col];
	int matSize = row*col;
	int index = 0;
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			mat[i][j] = c.access(index) + c.access(index+matSize);
			index++;
		}
	}
	cout<< "output matrice: " <<endl;
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			cout<< mat[i][j] << ",";
		}
		cout<< "\n" ;
	}
}
