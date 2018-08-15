#include <iostream>
#include <fstream>
#include <memory>

#include "myLib/Computing.h"
#include "myLib/Data.h"

void sumMatrices(Computing &c, int row, int col); 

int main()
{
	int inputArr[24];
	std::ifstream inputFile("input.txt");
	if(inputFile.is_open()){	
		for(int i=0; i<24; ++i){
			inputFile >> inputArr[i];
		}
	}
	inputFile.close();

	Data d(3);
	d.distribute(inputArr, 24);

	Computing c(3, 0, 24);
	sumMatrices(c, 3, 4);
	/*std::unique_ptr<int[]> p;	
	p = c.access(9,6);
	for(int i=0; i<6; ++i){
		std::cout << p[i] << std::endl;
	}*/
	
    return 0;
}

void sumMatrices(Computing &c, int row, int col){
	int mat[row][col];
	int matSize = row*col;
	int index = 0;
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			mat[i][j] = c.access(index) + c.access(index+matSize);
			++index;
		}
	}
	std::cout<< "output matrice: " <<std::endl;
	for(int i=0; i<row; ++i){
		for(int j=0; j<col; ++j){
			std::cout<< mat[i][j] << ",";
		}
		std::cout<< "\n" ;
	}
}
