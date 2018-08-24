#include <iostream>
#include <string>

#include "myLib/Computing.h"
#include "myLib/Data.h"

const std::string MY_IP = "tcp://192.168.56.103:5551"; 
 
int main(){

	Data d(MY_IP);
	d.getMyShare();	

	return 0;
}
