#include "myLib/DataAdaptor.h"

#include <fstream>
#include <string>

#include <zmq.hpp>
 
DataAdaptor::DataAdaptor(){
	std::ifstream dpfile("myDataIP.txt");
	std::string dip = "";
	while(getline(dpfile, dip)){
		if(dip.size()>0)
			setDataIP(dip);
	}	
	dpfile.close();
}

int DataAdaptor::reqRepMyData(int index){
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REQ);
	std::cout << "Request Element to my Data Process " << std::endl;
	socket.connect (getDataIP());
	zmq::message_t request (2);		
	std::string str = std::to_string(index);
	memcpy (request.data (), str.c_str(), 2);
	socket.send (request);
	zmq::message_t reply;
	socket.recv (&reply);
	std::string d = std::string(static_cast<char*>(reply.data()), reply.size());
	return stoi(d);		
}

std::string DataAdaptor::getDataIP(){return dataIP;}
void DataAdaptor::setDataIP(const std::string &str){
	dataIP = str;
}
