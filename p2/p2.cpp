#include <iostream>
#include <string>

#include "myLib/Computing.h"
#include "myLib/Data.h"

const std::string MY_IP = "tcp://localhost:5551"; 
 
int main(){

	Data d(MY_IP);
	d.getMyShare();	
	
	Computing c(MY_IP);
	c.ready4DataReq(d.getMyPart());

	return 0;
}
