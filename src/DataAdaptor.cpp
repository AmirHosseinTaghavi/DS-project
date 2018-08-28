#include "include/DataAdaptor.h"

#include <fstream>
#include <string>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <zmq.hpp>

using namespace boost::interprocess;
 
namespace DAHelperNS {
	static int reqRep(const std::vector<std::string> &procIPs, int ownerRnk, int index){
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Request Element to Process " << ownerRnk << std::endl;
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


	static std::string getPort(const std::string &ip){
		int colonPos = ip.rfind(':');
		return ip.substr(colonPos+1, 4);
	}	
}

DataAdaptor::DataAdaptor(int procCount, int myRank, int inputSize){
	setMyRank(myRank);
	setProcCount(procCount);
	setInputSize(inputSize);
	std::ifstream pfile("procIPs.txt");
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			procIPs.push_back(ip);
	}	
	pfile.close();
	std::string sharedName = "data";
	setShdMem(sharedName);
}

DataAdaptor::DataAdaptor(){
}

DataAdaptor::~DataAdaptor(){
	shared_memory_object::remove(getShdMem().c_str());	
}

int DataAdaptor::get(int index){
	int eachShare = getInputSize()/getProcCount();
	int dataOwner = index/eachShare;
	if(dataOwner == myRank){
		std::cout << "Owner of Requested Element is This Machine" << std::endl; 
		return DAHelperNS::getSharedData(index, getShdMem());
	}else{
		std::cout << "Owner of Requested Element is Machine: " << dataOwner << std::endl;
		int data = DAHelperNS::reqRep(procIPs, dataOwner, index);
		std::cout << "Data Owner Replied -> " << data << std::endl;
		return data;
	}	
}

int DataAdaptor::calcGet(int index){
	int eachShare = getInputSize()/getProcCount();
	int myIndex = index % eachShare;
	return DAHelperNS::getSharedData(myIndex, getShdMem());
}

int DataAdaptor::getInputSize(){return inputSize;}
int DataAdaptor::getMyRank(){return myRank;}
int DataAdaptor::getProcCount(){return procCount;}
std::string DataAdaptor::getShdMem(){return shdMemStr;}
void DataAdaptor::setInputSize(int size){
	inputSize = size;
}
void DataAdaptor::setMyRank(int rnk){
	myRank = rnk;
}
void DataAdaptor::setProcCount(int cnt){
	procCount = cnt;
}
void DataAdaptor::setShdMem(std::string str){
	shdMemStr = str;
}
