

class ServerComm
{
private:
	int port;
	int socketFd;
public:
	ServerComm(int port);
	int newConnection(void);
};
