#include <zmq.hpp>
#include <iostream>

int main(){

	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REQ);
	socket.connect ("tcp://192.168.56.106:5552");
	zmq::message_t request (4);
	memcpy (request.data (), "cntr", 4);
	socket.send (request);
	zmq::message_t reply;
	socket.recv (&reply);
	std::string d = std::string(static_cast<char*>(reply.data()), reply.size());
	std::cout << "count of request to p3(data): " << d << std::endl;

	return 0;
}
