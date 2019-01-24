#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/Data.h"

const std::string DIST_PORT = "5551"; 
const std::string DATA_PORT = "5541"; 

int main(){

	Data d(256);
	d.receivData(DIST_PORT);	

	d.replytoReqs(DATA_PORT);

    return 0;
}
