#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <zmq.hpp>

#ifndef MYLIB_COMPUTING_H
#define MYLIB_COMPUTING_H
 
class Computing {
	
	public:
		Computing(int procCount, int myRank);
		int getInputSize();
		int getMyRank();
		int getProcCount();
		std::unique_ptr<int[]> getMyPart();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void distribute(const int *input, int count);
		void getMyShare();		
		std::unique_ptr<int[]> access(int index, int count);
		int access(int index);
		void ready4Req();		
	
	private:
		int inputSize;
		int myRank;
		int procCount;
		std::unique_ptr<int[]> myPart;
		std::vector <std::string> procIPs;

};
#endif
