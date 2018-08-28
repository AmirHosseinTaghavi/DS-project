#include "include/Computing.h"
#include "include/DataAdaptor.h"

#include <fstream>
#include <memory>
#include <string>

#include <zmq.hpp>
 
namespace helperNS {
	std::string getPort(const std::string &ip){
		int colonPos = ip.rfind(':');
		return ip.substr(colonPos+1, 4);
	}	
}

//Constructor for Computing processes
Computing::Computing(const DataAdaptor &da){
	adaptor = da;
	std::ifstream pfile("compIP.txt");
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			setCompIP(ip);
	}	
	pfile.close();
}

//access multiple elements
std::unique_ptr<int[]> Computing::access(int index, int count){
	//int eachShare = inputSize/procCount;
	std::unique_ptr<int[]> reqedPart(new int[count]);
	for(int i=0; i<count; ++i){
		std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
		reqedPart[i] = adaptor.get(index);	
		++index;
	}	
	return std::move(reqedPart);
}

//access one element
int Computing::access(int index){
	std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
	return adaptor.get(index);
}	

/*This method can be used in computing processes. When this method is called in a process, That process will
listen for access requests and if it receives any index, this method calculates A correct index of its node 
and then request it from data process in its node. Finally, it returns Correct data to the process that called 
access method.*/
void Computing::ready4AccReq(){
	std::cout << "This Process is Ready for Access Requests..." << std::endl;
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS::getPort(getCompIP());    
	socket1.bind (prefix + port);	
    while (true) {
        zmq::message_t req;
        socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		int index = stoi(d);
		std::cout << index << "th Element is Requested" << std::endl;
		int data = adaptor.calcGet(index);
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
std::string Computing::getCompIP(){return compIP;}
void Computing::setInputSize(int size){
	inputSize = size;
}
void Computing::setMyRank(int rnk){
	myRank = rnk;
}
void Computing::setProcCount(int cnt){
	procCount = cnt;
}
void Computing::setCompIP(const std::string &ip){
	compIP = ip;
}
