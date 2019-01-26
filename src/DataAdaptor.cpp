#include "include/DataAdaptor.h"

#include <fstream>
#include <string>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <zmq.hpp>

using namespace boost::interprocess;
 
namespace DAHelperNS {

	static int dataReq(const std::vector<std::string> &dataIPs, int ownerRnk
						, int index, int eachShare){
		int dataIndex = index % eachShare;		
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Request Element to node " << ownerRnk << std::endl;
		socket.connect (dataIPs[ownerRnk]);
		
		zmq::message_t req_type (4);		
		memcpy (req_type.data (), "load", 4);
		socket.send (req_type);
		zmq::message_t rtreply;
		socket.recv (&rtreply);	
		
		zmq::message_t request (2);
		std::string str = std::to_string(dataIndex);
		memcpy (request.data (), str.c_str(), 2);
		socket.send (request);
		zmq::message_t reply;
		socket.recv (&reply);
		std::string d = std::string(static_cast<char*>(reply.data()), reply.size());
		return stoi(d);			
	}

	static void remotStor(const std::vector<std::string> &dataIPs, int ownerRnk
						, int index, int val, int eachShare){
		int dataIndex = index % eachShare;		
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Store Element to node " << ownerRnk << std::endl;
		socket.connect (dataIPs[ownerRnk]);
		
		zmq::message_t req_type (4);		
		memcpy (req_type.data (), "stor", 4);
		socket.send (req_type);
		zmq::message_t rtreply;
		socket.recv (&rtreply);	
		
		zmq::message_t request (2);
		std::string str = std::to_string(dataIndex);
		memcpy (request.data (), str.c_str(), 2);
		socket.send (request);
		zmq::message_t reply;
		socket.recv (&reply);			

		zmq::message_t request1 (2);
		std::string str1 = std::to_string(val);
		memcpy (request1.data (), str1.c_str(), 2);
		socket.send (request1);
		zmq::message_t reply1;
		socket.recv (&reply1);			
	}

	static int getSharedData(int index, std::string sharedName){
		std::cout << "get index from shared memory... " << std::endl;
		shared_memory_object shdmem(open_only, sharedName.c_str(), read_write);		
		mapped_region region(shdmem, read_write);
		int *data = static_cast<int*>(region.get_address());
		return data[index];	
	}

	static void storSharedData(int index, int val, std::string sharedName){
		std::cout << "Store index to shared memory... " << std::endl;
		shared_memory_object shdmem(open_only, sharedName.c_str(), read_write);		
		mapped_region region(shdmem, read_write);
		int *data = static_cast<int*>(region.get_address());
		data[index] = val;	
	}
}

/*This method is responsible for providing data from Data processes for Computing
processes. nodesCount is the count of machines that participate in communication.
nodeRank is the rank of computing process which creates this object among all nodes.
compProcsFile is the name of the file that should consist of ips of all computing 
processes*/
DataAdaptor::DataAdaptor(int nodesCount, int nodeRank, int inputSize, const std::string &dataProcsFile){
	setNodeRank(nodeRank);
	setNodesCount(nodesCount);
	setInputSize(inputSize);
	std::ifstream pfile(dataProcsFile);
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			dataProcIPs.push_back(ip);
	}	
	pfile.close();
	std::string sharedName = "data";
	setShdName(sharedName);
}

DataAdaptor::DataAdaptor(){
}

/*This method calculates the owner machine of requested index and if it's in 
the node of method's caller process the data will be received through shared
space. otherwise request it to Data process of owner node*/
int DataAdaptor::get(int index){
	int eachShare = getInputSize()/getNodesCount();
	int dataOwner = index/eachShare;
	if(dataOwner == getNodeRank()){
		std::cout << "Owner of Requested Element is This Machine" << std::endl; 
		return localGet(index);
	}else{
		std::cout << "Owner of Requested Element is Machine: " << dataOwner << std::endl;
		int data = DAHelperNS::dataReq(dataProcIPs, dataOwner, index, eachShare);
		std::cout << "Data Owner Replied -> " << data << std::endl;
		return data;
	}	
}

void DataAdaptor::stor(int index, int val){
	int eachShare = getInputSize()/getNodesCount();
	int dataOwner = index/eachShare;
	if(dataOwner == getNodeRank()){
		std::cout << "Owner of Store Element is This Machine" << std::endl; 
		localStor(index, val);
	}else{
		std::cout << "Owner of Store Element is Machine: " << dataOwner << std::endl;
		DAHelperNS::remotStor(dataProcIPs, dataOwner, index, val, eachShare);
		std::cout << "Store operation applied!" << std::endl;
	}	
}


/*This method receives global index of data and calculates its local index
and then return the correct data from shared memory space*/
int DataAdaptor::localGet(int index){
	int eachShare = getInputSize()/getNodesCount();
	int thisIndex = index % eachShare;
	return DAHelperNS::getSharedData(thisIndex, getShdName());
}

void DataAdaptor::localStor(int index, int val){
	int eachShare = getInputSize()/getNodesCount();
	int thisIndex = index % eachShare;
	DAHelperNS::storSharedData(thisIndex, val, getShdName());
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
