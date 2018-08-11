#include <iostream>
#include <string>

#include "myLib/Computing.h"
 
int main(){

	Computing comp(3, 1);
	
	comp.getMyShare();

	comp.ready4Req();

	return 0;
}
