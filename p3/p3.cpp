#include<iostream>
#include <zmq.hpp>
#include <string>

using namespace std;

const int EACHSHARE = 4;
 
int main(){
	int *myPart = new int[EACHSHARE];

	zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5542");	
	int cnt = 0;
    while (cnt != EACHSHARE) {
        zmq::message_t msg;
        socket.recv (&msg);
		string d = string(static_cast<char*>(msg.data()), msg.size());
		//cout << d << endl;
		myPart[cnt] = stoi(d);
		if(!d.empty()) cnt++;		
        zmq::message_t reply (5);
        memcpy (reply.data (), "", 5);
        socket.send (reply);
    }
	
	cout << "This Process Recieved its Part of Input : " << endl;

	for(int i=0; i<4; i++){
		cout << myPart[i] << endl;
	}

	zmq::context_t context1 (1);
    zmq::socket_t socket1 (context1, ZMQ_REP);
    socket1.bind ("tcp://*:5552");	
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

	return 0;
}
