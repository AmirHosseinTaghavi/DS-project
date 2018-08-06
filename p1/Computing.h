#include <iostream>
#include <zmq.hpp>
#include <vector>
#include <string>

using namespace std;

#ifndef COMPUTING_H
#define COMPUTING_H
 
class Computing {
	private:
		int inputSize;
		int myRank;
		int procCount;
		int *myPart;
		vector <string> procIPs;

	public:
		Computing(int procCount, int myRank);
		void distribute(int *input, int count);
		void getMyShare();		
		int access(int index);
		void ready4Req();		
		int getInputSize();
		int getMyRank();
		int getProcCount();
		int* getMyPart();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void setMyPart(int *part);	
};
#endif
