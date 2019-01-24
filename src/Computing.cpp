#include "include/Computing.h"

#include <memory>
#include <string>

#include <zmq.hpp>
#include "include/DataAdaptor.h"
 
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
