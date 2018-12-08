#include "include/DataAdaptor.h"

#include <fstream>
#include <string>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <zmq.hpp>

using namespace boost::interprocess;
 
namespace DAHelperNS {

	static int compDataReq(const std::vector<std::string> &procIPs, int ownerRnk, int index){
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Request Element to node " << ownerRnk << std::endl;
		socket.connect (procIPs[ownerRnk]);
		zmq::message_t request (2);
		std::string str = std::to_string(index);
		memcpy (request.data (), str.c_str(), 2);
		socket.send (request);
		zmq::message_t reply;
		socket.recv (&reply);
		std::string d = std::string(static_cast<char*>(reply.data()), reply.size());
		return stoi(d);			
	}

	static int getSharedData(int index, std::string sharedName){
		std::cout << "get index from shared memory... " << std::endl;
		shared_memory_object shdmem(open_only, sharedName.c_str(), read_write);		
		mapped_region region(shdmem, read_write);
		int *data = static_cast<int*>(region.get_address());
		return data[index];	
	}

	static void updateReqCnt(){
		shared_memory_object shdCntr(open_only, "cntr", read_write);		
		mapped_region region(shdCntr, read_write);
		int *cntr = static_cast<int*>(region.get_address());
		cntr[0] = cntr[0] + 1;
		std::cout << "cntr: " << cntr[0] << std::endl;
	}
}

/*This method is responsible for providing data from Data processes for Computing
processes. nodesCount is the count of machines that participate in communication.
nodeRank is the rank of computing process which creates this object among all nodes.
compProcsFile is the name of the file that should consist of ips of all computing 
processes*/
DataAdaptor::DataAdaptor(int nodesCount, int nodeRank, int inputSize, const std::string &compProcsFile){
	setNodeRank(nodeRank);
	setNodesCount(nodesCount);
	setInputSize(inputSize);
	std::ifstream pfile(compProcsFile);
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			compProcIPs.push_back(ip);
	}	
	pfile.close();
	std::string sharedName = "data";
	setShdName(sharedName);
	shared_memory_object shdCntr(open_or_create, "cntr", read_write);
	shdCntr.truncate(20);	
	mapped_region region(shdCntr, read_write);
	int *cntr = static_cast<int*>(region.get_address());
	cntr[0] = 0;
}

DataAdaptor::DataAdaptor(){
}

/*as the destructor called, the shared memory space that allocated in the data
process will be deallocated*/
DataAdaptor::~DataAdaptor(){
	shared_memory_object::remove(getShdName().c_str());	
	shared_memory_object::remove("cntr");	
}

/*This method calculates the owner machine of requested index and if it's in 
the node of method's caller process the data will be received through shared
space. otherwise request it to computing process of owner node*/
int DataAdaptor::get(int index){
	int eachShare = getInputSize()/getNodesCount();
	int dataOwner = index/eachShare;
	if(dataOwner == getNodeRank()){
		std::cout << "Owner of Requested Element is This Machine" << std::endl; 
		return localGet(index);
	}else{
		std::cout << "Owner of Requested Element is Machine: " << dataOwner << std::endl;
		int data = DAHelperNS::compDataReq(compProcIPs, dataOwner, index);
		std::cout << "Data Owner Replied -> " << data << std::endl;
		return data;
	}	
}

/*This method receives global index of data and calculates its local index
and then return the correct data from shared memory space*/
int DataAdaptor::localGet(int index){
	int eachShare = getInputSize()/getNodesCount();
	int thisIndex = index % eachShare;
	DAHelperNS::updateReqCnt();
	return DAHelperNS::getSharedData(thisIndex, getShdName());
}

int DataAdaptor::getInputSize(){return inputSize;}
int DataAdaptor::getNodeRank(){return nodeRank;}
int DataAdaptor::getNodesCount(){return nodesCount;}
std::string DataAdaptor::getShdName(){return shdMemName;}
void DataAdaptor::setInputSize(int size){
	inputSize = size;
}
void DataAdaptor::setNodeRank(int rnk){
	nodeRank = rnk;
}
void DataAdaptor::setNodesCount(int cnt){
	nodesCount = cnt;
}
void DataAdaptor::setShdName(std::string str){
	shdMemName = str;
}
