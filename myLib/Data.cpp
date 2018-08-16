#include "myLib/Data.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <zmq.hpp>
 
namespace helperNS1 {

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

	std::string getPort(const std::string &ip){
		int colonPos = ip.rfind(':');
		return ip.substr(colonPos+1, 4);
	}	
}

//constructor for computing process(p11)
Data::Data(int procCount){
	setProcCount(procCount);
	std::ifstream pfile("dataProcIPs.txt");
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			procIPs.push_back(ip);
	}	
	pfile.close();
}

//constructor fot data processes
Data::Data(const std::string &ip){
	setMyIP(ip);
}

//Distribute input among data processes
void Data::distribute(const int *input, int count){
	std::cout << "Distribute Input Data:" << std::endl;	
	setInputSize(count);
	setPartCount(inputSize / procCount);
	for(int j=0; j<procCount; ++j){
		helperNS1::distto(procIPs[j], j, input, getPartCount()); 
	}	
}

//use in data process and process will be ready for geting its part
void Data::getMyShare(){	
	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS1::getPort(getMyIP());
    socket.bind (prefix + port);	
	int cnt = 0;
	bool isFirst = true;
    while (true) {
        zmq::message_t msg;
        socket.recv (&msg);
		std::string d = std::string(static_cast<char*>(msg.data()), msg.size());
		if(!d.empty() && isFirst){
			setPartCount(stoi(d));
			isFirst = false;
			myPart = std::unique_ptr<int[]>(new int[getPartCount()]);			
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
		if(cnt == getPartCount()) break;
    }
	std::cout << "This Process Recieved its Part of Input : " << std::endl;		
	for(int i=0; i<getPartCount(); ++i){
		std::cout << myPart[i] << std::endl;
	}
}

/*This method can be used in data processes. When this method is called in a process, That process will
listen for data requests and if it receives any index, this method returns data of its part to the computing
process that called reqRepMyData function.*/
void Data::ready4DataReq(){
	std::cout << "This Process is Ready for Data Requests..." << std::endl;
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";
	std::string port = helperNS1::getPort(getMyIP());    
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

int Data::getInputSize(){return inputSize;}
int Data::getProcCount(){return procCount;}
int Data::getPartCount(){return partCount;}
std::string Data::getMyIP(){return myIP;}
std::unique_ptr<int[]> Data::getMyPart(){return std::move(myPart);}
void Data::setInputSize(int size){
	inputSize = size;
}
void Data::setProcCount(int cnt){
	procCount = cnt;
}
void Data::setPartCount(int cnt){
	partCount = cnt;	
}
void Data::setMyIP(const std::string &ip){
	myIP = ip;
}
