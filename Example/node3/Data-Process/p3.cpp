#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/Data.h"
 
const std::string DIST_PORT = "5552"; 
const std::string DATA_PORT = "5542"; 

int main(){

	Data d(256);
	d.receivData(DIST_PORT);	

	d.replytoReqs(DATA_PORT);

    return 0;
}
