#ifndef __CLIENTCOMM_H__
#define __CLIENTCOMM_H__

#include "../util/communication.h"
#include <string>

class ClientComm : public Communication
{
public:
	ClientComm(int port);
	bool connectServer(std::string serverIp, int serverPort);
};

#endif
