#include "myLib/Computing.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <zmq.hpp>
 
namespace helperNS {

	void distto(const std::string &ip, int rnk, const int* input, int eachShare){
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Send " << (rnk+1) << "nd Part of Input to process " << rnk << std::endl;
		socket.connect (ip);
		
		zmq::message_t msgEachShare (2);		
		std::string strEachShare = std::to_string(eachShare);
		memcpy (msgEachShare.data (), strEachShare.c_str(), 2);
		socket.send (msgEachShare);
		zmq::message_t ESreply;
		socket.recv (&ESreply);	
		
		for(int i=eachShare*rnk; i<eachShare*rnk+eachShare; ++i){
			zmq::message_t request (2);		
			std::string str = std::to_string(input[i]);
			memcpy (request.data (), str.c_str(), 2);
			socket.send (request);
			zmq::message_t reply;
			socket.recv (&reply);
		}
	}

	int reqRep(const std::vector<std::string> &procIPs, int ownerRnk, int procIndex){
		zmq::context_t context (1);
		zmq::socket_t socket (context, ZMQ_REQ);
		std::cout << "Request Element to Process " << ownerRnk << std::endl;
		socket.connect (procIPs[ownerRnk]);
		zmq::message_t request (2);		
		std::string str = std::to_string(procIndex);
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

Computing::Computing(int procCount, int myRank){
	setMyRank(myRank);
	setProcCount(procCount);
	std::ifstream pfile("procIPs.txt");
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			procIPs.push_back(ip);
	}	
	pfile.close();
}

void Computing::distribute(const int *input, int count){
	std::cout << "Distribute Input Data:" << std::endl;	
	setInputSize(count);
	int eachShare = inputSize / procCount;
	std::cout << "Allocate First Part of Input to This" << std::endl;
	myPart = std::unique_ptr<int[]>(new int[eachShare]);
	for(int i=0; i<eachShare; ++i){
		myPart[i] = input[i];
	}
	for(int j=1; j<procCount; ++j){
		helperNS::distto(procIPs[j], j, input, eachShare); 
	}	
}

void Computing::getMyShare(){	
	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS::getPort(procIPs[myRank]);
    socket.bind (prefix + port);	
	int cnt = 0;
	bool isFirst = true;
	int eachShare = 0;
    while (true) {
        zmq::message_t msg;
        socket.recv (&msg);
		std::string d = std::string(static_cast<char*>(msg.data()), msg.size());
		if(!d.empty() && isFirst){
			eachShare = stoi(d);
			isFirst = false;
			myPart = std::unique_ptr<int[]>(new int[eachShare]);			
			zmq::message_t reply (5);
    	    memcpy (reply.data (), "", 5);
	        socket.send (reply);		
			continue;				
		}
		myPart[cnt] = stoi(d);
		if(!d.empty()) ++cnt;		
        zmq::message_t reply (5);
        memcpy (reply.data (), "", 5);
        socket.send (reply);
		if(cnt == eachShare) break;
    }
	setInputSize(getProcCount() * eachShare);
	std::cout << "This Process Recieved its Part of Input : " << std::endl;		
	for(int i=0; i<eachShare; ++i){
		std::cout << myPart[i] << std::endl;
	}
}

std::unique_ptr<int[]> Computing::access(int index, int count){
	int eachShare = inputSize/procCount;
	std::unique_ptr<int[]> reqedPart(new int[count]);	
	for(int i=0; i<count; ++i){
		std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
		int dataOwner = index/eachShare;
		int procIndex = index%eachShare;
		if(dataOwner == myRank){
			std::cout << "Owner of Requested Element is This Process: " << myPart[procIndex] 
						<< std::endl; 
			reqedPart[i] = myPart[procIndex];			
		}else{
			std::cout << "Owner of Requested Element is process: " << dataOwner << std::endl;
			int data = helperNS::reqRep(procIPs, dataOwner, procIndex);
			std::cout << "Data Owner Replied -> " << data << std::endl;
			reqedPart[i] = data;
		}	
		++index;
	}	
	return std::move(reqedPart);
}	

int Computing::access(int index){
	std::cout << "This Process Wants to Access " << index << "th Element of Input" << std::endl;	
	int eachShare = inputSize/procCount;
	int dataOwner = index/eachShare;
	int procIndex = index%eachShare;
	if(dataOwner == myRank){
		std::cout << "Owner of Requested Element is This Process" << std::endl; 
		return myPart[procIndex];
	}else{
		std::cout << "Owner of Requested Element is process: " << dataOwner << std::endl;
		int data = helperNS::reqRep(procIPs, dataOwner, procIndex);
		std::cout << "Data Owner Replied -> " << data << std::endl;
		return data;
	}	
}	

void Computing::ready4Req(){
	std::cout << "This Process is Ready for Requests..." << std::endl;
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS::getPort(procIPs[myRank]);    
	socket1.bind (prefix + port);	
    while (true) {
        zmq::message_t req;
        socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		int procIndex = stoi(d);
		std::cout << "my " << procIndex << "th Element is Requested" << std::endl;
		int data = myPart[procIndex];
		std::string dataStr = std::to_string(data);		
        zmq::message_t reply (5);
        memcpy (reply.data (), dataStr.c_str(), 5);
		std::cout << "Reply " << data << " to p1" << std::endl;
        socket1.send (reply);
    }
}

int Computing::getInputSize(){return inputSize;}
int Computing::getMyRank(){return myRank;}
int Computing::getProcCount(){return procCount;}
std::unique_ptr<int[]> Computing::getMyPart(){return std::move(myPart);}
void Computing::setInputSize(int size){
	inputSize = size;
}
void Computing::setMyRank(int rnk){
	myRank = rnk;
}
void Computing::setProcCount(int cnt){
	procCount = cnt;
}
