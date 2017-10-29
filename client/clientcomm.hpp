#ifndef __CLIENTCOMM_HPP__
#define __CLIENTCOMM_HPP__

#include "../util/communication.hpp"
#include <string>

class ClientComm : public Communication
{
public:
	ClientComm(void);
	ClientComm(int port);
	bool connectServer(std::string serverIp, int serverPort);
};

#endif
