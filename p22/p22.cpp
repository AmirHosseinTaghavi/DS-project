#include <iostream>
#include <string>

#include "myLib/Computing.h"
#include "myLib/DataAdaptor.h"
 
int main(){

	DataAdaptor da(3, 1, 24);
	Computing c(da);
	c.ready4AccReq();

	return 0;
}
