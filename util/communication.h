#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "../config/env.h"
#include <string>
#define COMM_MESSAGE_BEGIN 	'\x2'

/*
namespace comm_protocol {
	enum messages {
		SEND_FILE //...
	}
}
*/

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

#endif
