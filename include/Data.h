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
		Data(int procCount);
		Data(const std::string &ip);
		int getInputSize();
		int getProcCount();
		int getPartCount();
		std::string getMyIP();
		std::string getShdMem();
		void setInputSize(int size);
		void setProcCount(int cnt);
		void setPartCount(int cnt);
		void setMyIP(const std::string &ip);
		void setShdMem(std::string str);
		void distribute(const int *input, int count);
		void getMyShare();		
	
	private:
		int inputSize;
		int procCount;
		int partCount;
		std::string myIP;
		std::string shdMemStr;
		std::vector <std::string> procIPs;

};

#endif
