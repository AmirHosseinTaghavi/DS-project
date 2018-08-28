#include <iostream>
#include <string>

#include "include/Computing.h"
#include "include/DataAdaptor.h"
 
int main(){

	DataAdaptor da(3, 2, 24);	
	Computing c(da);
	c.ready4AccReq();

	return 0;
}
