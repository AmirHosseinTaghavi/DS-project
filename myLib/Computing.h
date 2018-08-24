#include <iostream>
#include <memory>
#include <string>

#include "myLib/DataAdaptor.h"
#include <zmq.hpp>

#ifndef MYLIB_COMPUTING_H
#define MYLIB_COMPUTING_H
 
class Computing {
	
	public:
		Computing(const DataAdaptor &da);
		int getInputSize();
		int getMyRank();
		int getProcCount();
		std::string getCompIP();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void setCompIP(const std::string &ip);
		std::unique_ptr<int[]> access(int index, int count);
		int access(int index);
		void ready4AccReq();
	
	private:
		int inputSize;
		int myRank;
		int procCount;
		std::string compIP;
		DataAdaptor adaptor;
};
#endif
