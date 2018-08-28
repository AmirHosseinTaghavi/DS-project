#include <iostream>
#include <string>

#include <zmq.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#ifndef MYLIB_DATAADAPTOR_H
#define MYLIB_DATAADAPTOR_H

class DataAdaptor {
	
	public:
		DataAdaptor(int procCount, int myRank, int inputSize);
		DataAdaptor();
		~DataAdaptor();
		int getInputSize();
		int getMyRank();
		int getProcCount();
		std::string getShdMem();
		void setInputSize(int size);
		void setMyRank(int rnk);
		void setProcCount(int cnt);
		void setShdMem(std::string str);
		int get(int index);		
		int calcGet(int index);

	private:
		int inputSize;
		int myRank;
		int procCount;
		std::string shdMemStr;
		std::vector <std::string> procIPs;
};

#endif
