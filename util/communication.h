
/*
	Interface/Class for communication class with sockets
	ServerComm and ClientComm inherity by this class
*/

#include "../config/env.h"

#define COMM_MESSAGE_BEGIN 	'\x2'

namespace comm_protocol {
	enum messages {
		SEND_FILE, //...
	}
}


class Communication
{
protected:
	int port;
	int socketFd;
public:
	Communication();
	bool send(std::string message);
	std::string receive(void);
};
