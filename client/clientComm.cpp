#include "clientComm.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

ClientComm::ClientComm(int clientPort)
{
	this->port = clientPort;
	struct sockaddr_in clientAddr;

	if ((this->socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	  printf("[cliente_tcp]~: erro iniciando socket.\n");

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(clientPort);

	if ((bind(this->socketFd, (struct sockaddr *) &clientAddr, sizeof(clientAddr))) != 0)
		printf("[cliente_tcp]~: erro no bind.\n");

	printf("Client online at: %s\n", inet_ntoa(clientAddr.sin_addr));
}

bool ClientComm::connectServer(std::string serverIp, int serverPort)
{
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
	serverAddr.sin_port = htons(serverPort);

	if(connect(this->socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
		printf("~[client_tcp]: erro na conexao com o servidor %s.\n", serverIp.c_str());

	printf("Client connect with: %s\n", inet_ntoa(serverAddr.sin_addr));
	return true;
}
