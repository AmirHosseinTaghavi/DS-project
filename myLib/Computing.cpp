#include "Computing.h"
#include <zmq.hpp>
#include <string>
#include <vector>
#include <fstream>
 
using namespace std;
void distto(string ip,int rnk, int* input, int eachShare);
int reqRep(vector<string> procIPs, int ownerRnk, int procIndex);
string getPort(string ip);

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
	for(int j=1; j<procCount; j++){
		distto(procIPs[j], j, input, eachShare); 
	}	
}

void Computing::getMyShare(){	
	int EachShare;
	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
	string prefix = "tcp://*:";
	string port = getPort(procIPs[myRank]);
    socket.bind (prefix + port);	
	int cnt = 0;
	bool isFirst = true;
    while (true) {
        zmq::message_t msg;
        socket.recv (&msg);
		string d = string(static_cast<char*>(msg.data()), msg.size());
		if(!d.empty() && isFirst){
			EachShare = stoi(d);
			isFirst = false;
			myPart = new int[EachShare];			
			zmq::message_t reply (5);
    	    memcpy (reply.data (), "", 5);
	        socket.send (reply);		
			continue;				
		}
		myPart[cnt] = stoi(d);
		if(!d.empty()) cnt++;		
        zmq::message_t reply (5);
        memcpy (reply.data (), "", 5);
        socket.send (reply);
		if(cnt == EachShare) break;
    }
	setInputSize(getProcCount() * EachShare);
	cout << "This Process Recieved its Part of Input : " << endl;		
	for(int i=0; i<EachShare; i++){
		cout << getMyPart()[i] << endl;
	}
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

void Computing::ready4Req(){
	cout << "This Process is Ready for Requests..." << endl;
	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
	string prefix = "tcp://*:";
	string port = getPort(procIPs[myRank]);    
	socket1.bind (prefix + port);	
    while (true) {
        zmq::message_t req;
        socket1.recv (&req);
		string d = string(static_cast<char*>(req.data()), req.size());
		int procIndex = stoi(d);
		cout << "my " << procIndex << "th Element is Requested" << endl;
		int data = myPart[procIndex];
		string dataStr = to_string(data);		
        zmq::message_t reply (5);
        memcpy (reply.data (), dataStr.c_str(), 5);
		cout << "Reply " << data << " to p1" << endl;
        socket1.send (reply);
    }
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
	
	zmq::message_t msgEachShare (2);		
	string strEachShare = to_string(eachShare);
	memcpy (msgEachShare.data (), strEachShare.c_str(), 2);
    socket.send (msgEachShare);
	zmq::message_t ESreply;
	socket.recv (&ESreply);	
	
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

string getPort(string ip){
	int colonPos = ip.rfind(':');
	return ip.substr(colonPos+1, 4);
}
