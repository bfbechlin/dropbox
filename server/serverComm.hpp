#ifndef __SERVERCOMM_HPP__
#define __SERVERCOMM_HPP__

#include "../util/communication.hpp"
#include <string>

class ServerComm: public Communication
{
public:
	ServerComm(int port);
	ServerComm(int port, int socketFd);
	ServerComm newConnection(void);
};

#endif
