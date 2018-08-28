#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/Data.h"

const std::string MY_IP = "tcp://192.168.56.105:5551"; 
 
int main(){

	Data d(MY_IP);
	d.getMyShare();	

	return 0;
}
