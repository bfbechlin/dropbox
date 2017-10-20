#include <string>

class ServerComm
{
private:
	int port;
	int socketFd;
public:
	ServerComm(int port);
	ServerComm(int port, int socketFd);
	ServerComm newConnection(void);
	bool send(std::string message);
	std::string receive(void);
};
