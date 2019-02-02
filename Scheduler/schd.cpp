#include <iostream>
#include <zmq.hpp>
#include "include/Sender.h"
#include "include/Message.h"

int main(){
	
	Command newCommand;
	newCommand.Type = "run";
	newCommand.PID = 0;
	newCommand.DestIP = "192.168.56.103";
	newCommand.StartIndex = 0;
	newCommand.EndIndex = 4;
	
	Sender sender;
	sender.Send(newCommand.DestIP, newCommand);

	sleep(11);	

	Command newCommand1;
	newCommand1.Type = "migrate";
	newCommand1.PID = 0;
	newCommand1.DestIP = "192.168.56.102";

	sender.Send("192.168.56.103", newCommand1);

	return 0;
}
