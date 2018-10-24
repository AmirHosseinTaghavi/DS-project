#include <iostream>
#include <string>

#include "include/Data.h"

const std::string MY_PORT = "5550"; 

int main(){

	Data d(256);
	d.receivData(MY_PORT);	

    return 0;
}
