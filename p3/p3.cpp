#include<iostream>
#include "Computing.h"
#include <string>

using namespace std;
 
int main(){
	
	Computing comp(3, 2);
	
	comp.getMyShare();

	comp.ready4Req();

	return 0;
}
