#include <iostream>
#include <fstream>

#include "include/Computing.h"
#include "include/Data.h"

const std::string MY_IP = "tcp://192.168.56.101:5550"; 

int main()
{
	Data d(MY_IP);
	d.getMyShare();	

    return 0;
}
