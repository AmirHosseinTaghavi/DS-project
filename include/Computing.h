#include <iostream>
#include <memory>
#include <string>

#include "include/DataAdaptor.h"
#include <zmq.hpp>

#ifndef MYLIB_COMPUTING_H
#define MYLIB_COMPUTING_H
 
class Computing {
	
	public:
		Computing(const DataAdaptor &da);
		std::unique_ptr<int[]> access(int index, int count);
		int access(int index);
	
	private:
		DataAdaptor adaptor;
};
#endif
