#include "myLib/DataAdaptor.h"

#include <fstream>
#include <string>

#include <zmq.hpp>
 
namespace DAHelperNS {
	int reqRep(const std::vector<std::string> &procIPs, int ownerRnk, int index){
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

	int reqRepMyData(const std::string &ip, int index){
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Request Element to my Data Process " << std::endl;
		socket.connect (ip);
		zmq::message_t request (2);		
		std::string str = std::to_string(index);
		memcpy (request.data (), str.c_str(), 2);
		socket.send (request);
		zmq::message_t reply;
		socket.recv (&reply);
		std::string d = std::string(static_cast<char*>(reply.data()), reply.size());
		return stoi(d);	
	}


	std::string getPort(const std::string &ip){
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
	std::ifstream dpfile("myDataIP.txt");
	std::string dip = "";
	while(getline(dpfile, dip)){
		if(dip.size()>0)
			setDataIP(dip);
	}	
	dpfile.close();
}

DataAdaptor::DataAdaptor(){
}

int DataAdaptor::get(int index){
	int eachShare = getInputSize()/getProcCount();
	int dataOwner = index/eachShare;
	if(dataOwner == myRank){
		std::cout << "Owner of Requested Element is This Machine" << std::endl; 
		return DAHelperNS::reqRepMyData(getDataIP(), index);
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
	return DAHelperNS::reqRepMyData(getDataIP(), myIndex);
}

std::string DataAdaptor::getDataIP(){return dataIP;}
int DataAdaptor::getInputSize(){return inputSize;}
int DataAdaptor::getMyRank(){return myRank;}
int DataAdaptor::getProcCount(){return procCount;}
void DataAdaptor::setInputSize(int size){
	inputSize = size;
}
void DataAdaptor::setMyRank(int rnk){
	myRank = rnk;
}
void DataAdaptor::setProcCount(int cnt){
	procCount = cnt;
}
void DataAdaptor::setDataIP(const std::string &str){
	dataIP = str;
}
