#include "../util/communication.h"
#include <string>

class ClientComm : public Communication
{
private:
	int port;
	int socketFd;
public:
	ClientComm(int port);
	bool connectServer(std::string serverIp, int serverPort);
	bool send(std::string message);
	std::string receive(void);
};
