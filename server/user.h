#ifndef __USER_H__
#define __USER_H__

#include "../config/env.h"
#include <string>

#define COMM_FILE_CHUNK_SIZE 8192 //bytes
#define COMM_MESSAGE_BEGIN 	'\x2'
#define COMM_FILE_BEGIN 	'\x3'

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
	bool sendFile(std::string fileSourcePath);
	bool receiveFile(std::string fileDestPath);
};

#endif
