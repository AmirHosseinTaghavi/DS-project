#include <iostream>
#include <memory>
#include <string>
#include <vector>

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
		std::unique_ptr<int[]> getMyPart();
		void setInputSize(int size);
		void setProcCount(int cnt);
		void setPartCount(int cnt);
		void setMyIP(const std::string &ip);
		void distribute(const int *input, int count);
		void getMyShare();		
		void ready4DataReq();
	
	private:
		int inputSize;
		int procCount;
		int partCount;
		std::string myIP;
		std::unique_ptr<int[]> myPart;
		std::vector <std::string> procIPs;

};

#endif
