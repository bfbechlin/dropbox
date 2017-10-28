#include "serverComm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ServerComm::ServerComm(int port)
{
	this->port = port;
	struct sockaddr_in serverAddr;

	if ((this->socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "[server]~: ERROR on opening socket.\n");
		exit(1);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serverAddr.sin_zero), 8);

	if (bind(this->socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		fprintf(stderr, "[server]~: ERROR on binding socket with port %i.\n", this->port);
		exit(1);
	}
}

ServerComm::ServerComm(int port, int socketFd){
	this->port = port;
	this->socketFd = socketFd;
}

ServerComm ServerComm::newConnection(void)
{
	int clientSocket;
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	if((listen(this->socketFd, 8)) != 0)
	{
		fprintf(stderr, "[server]~: ERROR on listing new connections.\n");
		exit(1);
	}

	if((clientSocket = accept(this->socketFd, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrLen)) < 0)
	{
		fprintf(stderr, "[server]~: ERROR on accept connection from client.\n");
		exit(1);
	}

	fprintf(stderr, "[server]~: SUCCESS accepted connection with %s\n", inet_ntoa(clientAddr.sin_addr));
	return ServerComm(this->port, clientSocket);
}
