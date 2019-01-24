#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <zmq.hpp>

#ifndef MYLIB_DATA_H
#define MYLIB_DATA_H

class Data {
	
	public:
		Data(int procCount, const std::string &distFile);
		Data(int sharedSize);
		~Data();
		int getInputSize();
		int getProcCount();
		int getPartCount();
		std::string getShdName();
		int getAccessCnt();
		void setInputSize(int size);
		void setProcCount(int cnt);
		void setPartCount(int cnt);
		void setShdName(std::string str);
		void setAccessCnt(int cnt);
		void distribute(const int *input, int count);
		void receivData(const std::string &procPort);
		void replytoReqs(const std::string &port);		
	
	private:
		int inputSize;
		int procCount;
		int partCount;
		int* dataPartArr; 
		std::string shdMemName;
		std::vector <std::string> distIPs;
		int accessCnt;
};

#endif
