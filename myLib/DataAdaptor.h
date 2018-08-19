#include <iostream>
#include <string>

#include <zmq.hpp>

#ifndef MYLIB_DATAADAPTOR_H
#define MYLIB_DATAADAPTOR_H

class DataAdaptor {
	
	public:
		DataAdaptor(int procCount, int myRank, int inputSize);
		DataAdaptor();
		std::string getDataIP();
		int getInputSize();
		int getMyRank();
		int getProcCount();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void setDataIP(const std::string &str);
		int get(int index);		
		int calcGet(int index);

	private:
		int inputSize;
		int myRank;
		int procCount;
		std::vector <std::string> procIPs;
		std::string dataIP;
};

#endif
