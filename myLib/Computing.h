#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <zmq.hpp>

#ifndef MYLIB_COMPUTING_H
#define MYLIB_COMPUTING_H
 
class Computing {
	
	public:
		Computing(int procCount, int myRank, int inputSize);
		int getInputSize();
		int getMyRank();
		int getProcCount();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		std::unique_ptr<int[]> access(int index, int count);
		int access(int index);
		void ready4AccReq();
	
	private:
		int inputSize;
		int myRank;
		int procCount;
		std::vector <std::string> procIPs;

};
#endif
