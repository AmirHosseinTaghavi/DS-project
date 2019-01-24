#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/DataAdaptor.h"

int main(){

	DataAdaptor da(3, 2, 24, "dataProcIPs.txt");	
	Computing c(da);

	return 0;
}
