#include "serverComm.h"
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
        printf("ERROR opening socket");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serverAddr.sin_zero), 8);

	if (bind(this->socketFd, (struct sockaddr *) &serverAddr,
		sizeof(serverAddr)) < 0)
		printf("ERROR on binding");

	printf("Server online at: %s\n", inet_ntoa(serverAddr.sin_addr));
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
		printf("[servidor_tcp]~: erro ao realizar listen.");

	if((clientSocket = accept(this->socketFd, (struct sockaddr *) &clientAddr,
		(socklen_t *) &clientAddrLen)) < 0)
		printf("[servidor_tcp]~: erro ao aceitar a conexao com o cliente.");

	printf("Accepted connection with %s\n", inet_ntoa(clientAddr.sin_addr));
	return ServerComm(this->port, clientSocket);
}
