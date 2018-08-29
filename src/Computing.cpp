#include "include/Computing.h"
#include "include/DataAdaptor.h"

#include <memory>
#include <string>

#include <zmq.hpp>
 
/*Constructor for Computing process. This object is composed of DataAdaptor 
object which does providing data.*/
Computing::Computing(const DataAdaptor &da){
	adaptor = da;
}

//access multiple elements
std::unique_ptr<int[]> Computing::access(int index, int count){
	std::unique_ptr<int[]> elements(new int[count]);
	for(int i=0; i<count; ++i){
		std::cout << "This Process Wants to Access Element number" << index 
					<< " of Input" << std::endl;	
		elements[i] = adaptor.get(index);	
		++index;
	}	
	return std::move(elements);
}

//access one element
int Computing::access(int index){
	std::cout << "This Process Wants to Access Element number" << index 
				<< " of Input" << std::endl;
	return adaptor.get(index);
}	

/*This method can be used in computing processes. When this method is called in 
a process, That process will listen for data access requests and if it receives
any index, it access the data through DataAdaptor object.*/
void Computing::replytoReqs(const std::string &port){
	std::cout << "This Process is Ready for Access Requests..." << std::endl;
	zmq::context_t context1 (1);
	zmq::socket_t socket1 (context1, ZMQ_REP);
	std::string prefix = "tcp://*:";    
	socket1.bind (prefix + port);	
	while (true) {
		zmq::message_t req;
		socket1.recv (&req);
		std::string d = std::string(static_cast<char*>(req.data()), req.size());
		int index = stoi(d);
		std::cout << "Element number" << index << " is Requested" << std::endl;
		int data = adaptor.localGet(index);
		std::string dataStr = std::to_string(data);		
		zmq::message_t reply (5);
		memcpy (reply.data (), dataStr.c_str(), 5);
		std::cout << "Reply " << data << std::endl;
		socket1.send (reply);
	}
}
