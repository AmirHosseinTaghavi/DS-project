#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/Data.h"

const std::string MY_PORT = "5551"; 
 
int main(){

	Data d(256);
	d.receivData(MY_PORT);	

	return 0;
}