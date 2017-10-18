#include <string>

class ClientComm
{
private:
	int port;
	int socketFd;
public:
	ClientComm(int port);
	bool connectServer(std::string serverIp, int serverPort);
};
