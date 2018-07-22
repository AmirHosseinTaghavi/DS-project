#include "Computing.h"
#include <zmq.hpp>
#include <string>
#include <vector>
#include <fstream>
 
using namespace std;
void distto(string ip,int rnk, int* input, int eachShare);
int reqRep(vector<string> procIPs, int ownerRnk, int procIndex);

Computing::Computing(int procCount, int myRank){
	setMyRank(myRank);
	setProcCount(procCount);
	ifstream pfile("procIPs.txt");
	string ip;
	while(getline(pfile, ip)){
		if(ip.size()>0)
			procIPs.push_back(ip);
	}	
	pfile.close();

}
void Computing::distribute(int *input, int count){
	cout << "Distribute Input Data:" << endl;	
	setInputSize(count);
	int eachShare = inputSize / procCount;
	cout << "Allocate First Part of Input to This" << endl;
	myPart = new int[eachShare];
	for(int i=0; i<eachShare; i++){
		myPart[i] = input[i];
	}
	distto("tcp://localhost:5541",1, input, eachShare); 
	distto("tcp://localhost:5542",2, input, eachShare);
}
int Computing::access(int index){
	cout << "This Process Wants to Access " << index << "th Element of Input" << endl;	
	int eachShare = inputSize/procCount;
	int dataOwner = index/eachShare;
	int procIndex = index%eachShare;
	if(dataOwner == myRank){
		cout << "Owner of Requested Element is This Process" << endl; 
		return myPart[procIndex];
	}else{
		cout << "Owner of Requested Element is process: " << dataOwner << endl;
		int data = reqRep(procIPs, dataOwner, procIndex);
		cout << "Data Owner Replied -> " << data << endl;
		return data;
	}	
	return 0;
}	
int Computing::getInputSize(){return inputSize;}
int Computing::getMyRank(){return myRank;}
int Computing::getProcCount(){return procCount;}
int* Computing::getMyPart(){return myPart;}
void Computing::setInputSize(int size){
	inputSize = size;
}
void Computing::setMyRank(int rnk){
	myRank = rnk;
}
void Computing::setProcCount(int cnt){
	procCount = cnt;
}
void Computing::setMyPart(int *part){
	myPart = part;
}	

void distto(string ip, int rnk, int* input, int eachShare){
	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    cout << "Send " << (rnk+1) << "nd Part of Input to process " << rnk << endl;
    socket.connect (ip);	
	for(int i=eachShare*rnk; i<eachShare*rnk+eachShare; i++){
		zmq::message_t request (2);		
		string str = to_string(input[i]);
		memcpy (request.data (), str.c_str(), 2);
    	socket.send (request);
		zmq::message_t reply;
	    socket.recv (&reply);
	}
}

int reqRep(vector<string> procIPs, int ownerRnk, int procIndex){
	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    cout << "Request Element to Process " << ownerRnk << endl;
    socket.connect (procIPs[ownerRnk]);
	zmq::message_t request (2);		
	string str = to_string(procIndex);
	memcpy (request.data (), str.c_str(), 2);
    socket.send (request);
	zmq::message_t reply;
	socket.recv (&reply);
	string d = string(static_cast<char*>(reply.data()), reply.size());
	return stoi(d);			
}
