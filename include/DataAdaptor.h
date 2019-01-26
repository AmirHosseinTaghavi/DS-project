#include <iostream>
#include <string>

#include <zmq.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#ifndef MYLIB_DATAADAPTOR_H
#define MYLIB_DATAADAPTOR_H

class DataAdaptor {
	
	public:
		DataAdaptor(int nodesCount, int nodeRank, int inputSize, const std::string &dataProcsFile);
		DataAdaptor();
		int getInputSize();
		int getNodeRank();
		int getNodesCount();
		std::string getShdName();
		void setInputSize(int size);
		void setNodeRank(int rnk);
		void setNodesCount(int cnt);
		void setShdName(std::string str);
		int get(int index);		
		void stor(int index, int val);
		int localGet(int index);
		void localStor(int index, int val);

	private:
		int inputSize;
		int nodeRank;
		int nodesCount;
		std::string shdMemName;
		std::vector <std::string> dataProcIPs;
};

#endif
