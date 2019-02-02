#include <iostream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <queue>
#include <functional>

#include "include/Computing.h"
#include "include/Data.h"
#include "include/DataAdaptor.h"
#include "include/Client.h"

using namespace std;

void foo(Computing &c, int x)
{
	cout<< "foo started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+8)%24);
	sleep(2);
	cout << "foo finished" <<endl;
}

void foo2(Computing &c, int x)
{
	cout<< "foo2 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+8)%24);
	sleep(2);
	cout << "foo2 finished" <<endl;
}

void foo3(Computing &c, int x)
{
	cout<< "foo3 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+8)%24);
	sleep(2);
	cout << "foo3 finished" <<endl;
}

void foo4(Computing &c, int x)
{
	cout<< "foo4 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+8)%24);
	sleep(2);
	cout << "foo4 finished" <<endl;
}

void foo5(Computing &c, int x)
{
	cout<< "foo5 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+8)%24);
	sleep(2);
	cout << "foo5 finished" <<endl;
}

void bar(Computing &c, int x)
{
	cout<< "bar started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+4)%24);
	sleep(2);
	cout << "bar finished" <<endl;
}

void bar2(Computing &c, int x)
{
	cout<< "bar2 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+4)%24);
	sleep(2);
	cout << "bar2 finished" <<endl;
}

void bar3(Computing &c, int x)
{
	cout<< "bar3 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+4)%24);
	sleep(2);
	cout << "bar3 finished" <<endl;
}

void bar4(Computing &c, int x)
{
	cout<< "bar4 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+4)%24);
	sleep(2);
	cout << "bar4 finished" <<endl;
}

void bar5(Computing &c, int x)
{
	cout<< "bar5 started " <<endl;
	c.access(x);
	sleep(2);
	c.access((x+4)%24);
	sleep(2);
	cout << "bar5 finished" <<endl;
}

//void sumMatrices(Computing &c, int row, int col); 

int main()
{
	int inputArr[24];
	ifstream inputFile("input.txt");
	if(inputFile.is_open()){	
		for(int i=0; i<24; ++i){
			inputFile >> inputArr[i];
		}
	}
	inputFile.close();

	Data d(3, "distIPs.txt");
	d.distribute(inputArr, 24);
	
	DataAdaptor da(3, 0, 24, "dataProcIPs.txt");	
	Computing c(da);	

	vector<function<void()>> q1, q2;
	q1.push_back( [&] { foo(c, 1); } );	
	q1.push_back( [&] { foo2(c, 3); } );	
	q1.push_back( [&] { foo3(c, 5); } );	
	q1.push_back( [&] { foo4(c, 7); } ); 
	q1.push_back( [&] { foo5(c , 9); } );
	q2.push_back( [&] { bar(c, 2); } );	
	q2.push_back( [&] { bar2(c, 4); } );	
	q2.push_back( [&] { bar3(c, 6); } );	
	q2.push_back( [&] { bar4(c, 8); } ); 
	q2.push_back( [&] { bar5(c, 10); } );

	Client client(q1, q2);
	client.Start();
	
    return 0;
}

/*void sumMatrices(Computing &c, int row, int col){
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
}*/

/*
sumMatrices(c, 3, 4);
std::unique_ptr<int[]> p;	
	p = c.access(9,6);
	for(int i=0; i<6; ++i){
		std::cout << p[i] << std::endl;
	}*/
