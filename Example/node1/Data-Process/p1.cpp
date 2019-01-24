#include <iostream>
#include <string>

#include "include/Data.h"

const std::string DIST_PORT = "5550"; 
const std::string DATA_PORT = "5540"; 

int main(){

	Data d(256);
	d.receivData(DIST_PORT);	

	d.replytoReqs(DATA_PORT);

    return 0;
}
