#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/DataAdaptor.h"
 

int main(){

	DataAdaptor da(3, 1, 24, "dataProcIPs.txt");
	Computing c(da);

	return 0;
}
