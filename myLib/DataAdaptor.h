#include <iostream>
#include <string>

#include <zmq.hpp>

#ifndef MYLIB_DATAADAPTOR_H
#define MYLIB_DATAADAPTOR_H

class DataAdaptor {
	
	public:
		DataAdaptor();
		std::string getDataIP();
		void setDataIP(const std::string &str);
		int reqRepMyData(int index);		

	private:
		std::string dataIP;
};

#endif
