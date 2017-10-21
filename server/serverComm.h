#ifndef __SERVERCOMM_H__
#define __SERVERCOMM_H__

#include "../util/communication.h"
#include <string>

class ServerComm: public Communication
{
public:
	ServerComm(int port);
	ServerComm(int port, int socketFd);
	ServerComm newConnection(void);
};

#endif
