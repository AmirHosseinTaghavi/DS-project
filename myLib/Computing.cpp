#include "myLib/Computing.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <zmq.hpp>
 
namespace helperNS {

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

Computing::Computing(int procCount, int myRank, int inputSize){
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


Computing::Computing(const std::string &ip){
	setDataIP(ip);
}

std::unique_ptr<int[]> Computing::access(int index, int count){
	int eachShare = inputSize/procCount;
	std::unique_ptr<int[]> reqedPart(new int[count]);	
	for(int i=0; i<count; ++i){
		std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
		int dataOwner = index/eachShare;
		if(dataOwner == myRank){
			std::cout << "Owner of Requested Element is This Process" << std::endl; 
			reqedPart[i] = helperNS::reqRepMyData(getDataIP(), index);			
		}else{
			std::cout << "Owner of Requested Element is process: " << dataOwner << std::endl;
			int data = helperNS::reqRep(procIPs, dataOwner, index);
			std::cout << "Data Owner Replied -> " << data << std::endl;
			reqedPart[i] = data;
		}	
		++index;
	}	
	return std::move(reqedPart);
}

int Computing::access(int index){
	std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
	int eachShare = getInputSize()/getProcCount();
	int dataOwner = index/eachShare;
	if(dataOwner == myRank){
		std::cout << "Owner of Requested Element is This Process" << std::endl; 
		return helperNS::reqRepMyData(getDataIP(), index);
	}else{
		std::cout << "Owner of Requested Element is process: " << dataOwner << std::endl;
		int data = helperNS::reqRep(procIPs, dataOwner, index);
		std::cout << "Data Owner Replied -> " << data << std::endl;
		return data;
	}	
}	

void Computing::ready4AccReq(){
	std::cout << "This Process is Ready for Access Requests..." << std::endl;
	int eachShare = getInputSize() / getProcCount();
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS::getPort(procIPs[myRank]);    
	socket1.bind (prefix + port);	
    while (true) {
        zmq::message_t req;
        socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		int index = stoi(d);
		std::cout << index << "th Element is Requested" << std::endl;
		int myIndex = index % eachShare;
		int data = helperNS::reqRepMyData(getDataIP(), myIndex);
		std::string dataStr = std::to_string(data);		
        zmq::message_t reply (5);
        memcpy (reply.data (), dataStr.c_str(), 5);
		std::cout << "Reply " << data << std::endl;
        socket1.send (reply);
    }
}

void Computing::ready4DataReq(std::unique_ptr<int[]> myPart){
	std::cout << "This Process is Ready for Data Requests..." << std::endl;
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS::getPort(getDataIP());    
	socket1.bind (prefix + port);	
    while (true) {
        zmq::message_t req;
        socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		int index = stoi(d);
		std::cout << "my " << index << "th Element is Requested" << std::endl;
		int data = myPart[index];
		std::string dataStr = std::to_string(data);		
        zmq::message_t reply (5);
        memcpy (reply.data (), dataStr.c_str(), 5);
		std::cout << "Reply " << data << std::endl;
        socket1.send (reply);
    }
}

int Computing::getInputSize(){return inputSize;}
int Computing::getMyRank(){return myRank;}
int Computing::getProcCount(){return procCount;}
std::string Computing::getDataIP(){return myDataIP;}
void Computing::setInputSize(int size){
	inputSize = size;
}
void Computing::setMyRank(int rnk){
	myRank = rnk;
}
void Computing::setProcCount(int cnt){
	procCount = cnt;
}
void Computing::setDataIP(const std::string &ip){
	myDataIP = ip;
}
