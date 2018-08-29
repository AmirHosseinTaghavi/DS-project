#include <iostream>
#include <string>

#include <zmq.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#ifndef MYLIB_DATAADAPTOR_H
#define MYLIB_DATAADAPTOR_H

class DataAdaptor {
	
	public:
		DataAdaptor(int nodesCount, int nodeRank, int inputSize, const std::string &compProcsFile);
		DataAdaptor();
		~DataAdaptor();
		int getInputSize();
		int getNodeRank();
		int getNodesCount();
		std::string getShdName();
		void setInputSize(int size);
		void setNodeRank(int rnk);
		void setNodesCount(int cnt);
		void setShdName(std::string str);
		int get(int index);		
		int localGet(int index);

	private:
		int inputSize;
		int nodeRank;
		int nodesCount;
		std::string shdMemName;
		std::vector <std::string> compProcIPs;
};

#endif
