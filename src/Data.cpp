#include "include/Data.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <zmq.hpp>

using namespace boost::interprocess;
 
namespace DHelperNS {

	static void distto(const std::string &ip, int rnk, const int* input, int eachShare){
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
			zmq::message_t msg (2);		
			std::string str = std::to_string(input[i]);
			memcpy (msg.data (), str.c_str(), 2);
			socket.send (msg);
			zmq::message_t reply;
			socket.recv (&reply);
		}
	}
}

/*constructor for computing process which gets input and wants to distribute 
it among all machines. distfile should consist of ips of data processes*/
Data::Data(int procCount, const std::string &distFile){
	setProcCount(procCount);
	std::ifstream pfile(distFile);
	std::string ip = "";
	while(getline(pfile, ip)){
		if(ip.size()>0)
			distIPs.push_back(ip);
	}	
	pfile.close();
}

/*constructor for data processes. it creates shared memory space and then
allocates sharedSize bytes of data to that space.*/
Data::Data(int sharedSize){
	std::string shdMemName = "data";
	shared_memory_object shdmem(open_or_create, shdMemName.c_str(), read_write);
	shdmem.truncate(sharedSize);
	setShdName(shdMemName);
	accessCnt = 0;
}

Data::~Data(){
	shared_memory_object::remove(getShdName().c_str());
}

//Distribute input among data processes
void Data::distribute(const int *input, int count){
	std::cout << "Distribute Input Data:" << std::endl;	
	setInputSize(count);
	setPartCount(inputSize / procCount);
	for(int j=0; j<procCount; ++j){
		DHelperNS::distto(distIPs[j], j, input, getPartCount()); 
	}	
}

/*by calling this method, the data process receives its part of input
and allocates it to shared memory space.the port of caller process of 
the method should be passed as an argument*/
void Data::receivData(const std::string &procPort){
	shared_memory_object shdmem(open_only, getShdName().c_str(), read_write);	
	mapped_region region(shdmem, read_write);
	int *dataPart = static_cast<int*>(region.get_address());
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REP);
	std::string prefix = "tcp://*:";
	socket.bind (prefix + procPort);	
	int cnt = 0;
	bool isFirst = true;
	while (true) {
		zmq::message_t msg;
		socket.recv (&msg);
		std::string d = std::string(static_cast<char*>(msg.data()), msg.size());
		if(!d.empty() && isFirst){
			setPartCount(stoi(d));
			isFirst = false;
			zmq::message_t reply (5);
			memcpy (reply.data (), "", 5);
			socket.send (reply);		
			continue;				
		}
		dataPart[cnt] = stoi(d);
		if(!d.empty()) ++cnt;		
		zmq::message_t reply (5);
		memcpy (reply.data (), "", 5);
		socket.send (reply);
		if(cnt == getPartCount()) break;
	}
	std::cout << "This Process Recieved its Part of Input : " << std::endl;		
	for(int i=0; i<getPartCount(); ++i){
		std::cout << dataPart[i] << ",";
	}
	std::cout << "\n" ;
	std::cout << "Data is Allocated to Shared Memory Space." << std::endl;
}

/*This method can be used in Data processes. When this method is called in 
a process, That process will listen for data access requests and if it receives
any index, it returns that data.*/
void Data::replytoReqs(const std::string &port){
	std::cout << "This Process is Ready for Access Requests..." << std::endl;
	shared_memory_object shdmem(open_only, getShdName().c_str(), read_write);	
	mapped_region region(shdmem, read_write);
	int *dataPart = static_cast<int*>(region.get_address());
	zmq::context_t context1 (1);
	zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";    
	socket1.bind (prefix + port);
	bool isFirst = true;
	bool storIsFirst = true;
	int storIndex;
	int storVal;
	std::string req_type;	
	while (true) {
		zmq::message_t req;
		socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		if(!d.empty() && isFirst){
			req_type = d;
			if(req_type == "cntr"){
				std::cout << "Count of accesses is requested" << std::endl;
				std::string strCntr = std::to_string(accessCnt);
				zmq::message_t reply (strCntr.length());
				memcpy (reply.data (), strCntr.c_str(), strCntr.length());	
				socket1.send (reply);		
			}else{
				isFirst = false;
				zmq::message_t reply (5);
				memcpy (reply.data (), "", 5);
				socket1.send (reply);		
			}			
			continue;				
		}
		if(req_type == "load"){		
			int index = stoi(d);
			std::cout << "Element number" << index << " is Requested" << std::endl;
			int data = dataPart[index];
			++accessCnt;
			std::cout << "access count incremented!" << std::endl;
			std::string dataStr = std::to_string(data);		
			zmq::message_t reply (5);
			memcpy (reply.data (), dataStr.c_str(), 5);
			std::cout << "Reply " << data << std::endl;
			socket1.send (reply);
			isFirst = true;
		}
		else if(req_type == "stor"){
			if(storIsFirst){
				storIndex = stoi(d);
				zmq::message_t reply (5);
				memcpy (reply.data (), "", 5);
				socket1.send (reply);
				storIsFirst = false;		
			}else{
				storVal = stoi(d);
				dataPart[storIndex] = storVal;
				std::cout << "Element #" << storIndex << " changed to " 
							<< storVal << std::endl;
				zmq::message_t reply (5);
				memcpy (reply.data (), "", 5);
				socket1.send (reply);
				isFirst = true;
				storIsFirst = true;
			}
		}
	}
}

int Data::getInputSize(){return inputSize;}
int Data::getProcCount(){return procCount;}
int Data::getPartCount(){return partCount;}
std::string Data::getShdName(){return shdMemName;}
int Data::getAccessCnt(){return accessCnt;}
void Data::setInputSize(int size){
	inputSize = size;
}
void Data::setProcCount(int cnt){
	procCount = cnt;
}
void Data::setPartCount(int cnt){
	partCount = cnt;	
}
void Data::setShdName(std::string str){
	shdMemName = str;
}
void Data::setAccessCnt(int cnt){
	accessCnt = cnt;
}
