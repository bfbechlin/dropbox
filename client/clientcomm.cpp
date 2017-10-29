#include "clientcomm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

ClientComm::ClientComm(void)
{
	this->port = 0;
	struct sockaddr_in clientAddr;

	if ((this->socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "[client]~: ERROR on opening socket.\n");
		exit(1);
	}
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = this->port;

	if ((bind(this->socketFd, (struct sockaddr *) &clientAddr, sizeof(clientAddr))) != 0)
	{
		fprintf(stderr, "[client]~: ERROR on binding socket at port %i.\n", this->port);
		exit(1);
	}
}


ClientComm::ClientComm(int clientPort)
{
	this->port = clientPort;
	struct sockaddr_in clientAddr;

	if ((this->socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "[client]~: ERROR on opening socket.\n");
		exit(1);
	}
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(clientPort);

	if ((bind(this->socketFd, (struct sockaddr *) &clientAddr, sizeof(clientAddr))) != 0)
	{
		fprintf(stderr, "[client]~: ERROR on binding socket at port %i.\n", this->port);
		exit(1);
	}
}

bool ClientComm::connectServer(std::string serverIp, int serverPort)
{
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
	serverAddr.sin_port = htons(serverPort);

	if(connect(this->socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		fprintf(stderr, "[client]~: ERROR on open connection with server.\n");
		exit(1);
	}

	fprintf(stderr, "[client]~: SUCCESS connect with %s\n",  inet_ntoa(serverAddr.sin_addr));
	return true;
}
