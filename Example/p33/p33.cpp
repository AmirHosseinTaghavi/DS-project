#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/DataAdaptor.h"
 
const std::string MY_PORT = "5542"; 

int main(){

	DataAdaptor da(3, 2, 24, "compProcIPs.txt");	
	Computing c(da);
	c.replytoReqs(MY_PORT);

	return 0;
}
