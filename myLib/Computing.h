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
		Computing(const std::string &ip);
		int getInputSize();
		int getMyRank();
		int getProcCount();
		std::string getDataIP();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void setDataIP(const std::string &ip);		
		std::unique_ptr<int[]> access(int index, int count);
		int access(int index);
		void ready4AccReq();
	
	private:
		int inputSize;
		int myRank;
		int procCount;
		std::string myDataIP;
		std::vector <std::string> procIPs;

};
#endif
