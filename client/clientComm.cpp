#include "clientComm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#define BUFFER_LEN 50
#define COMM_MESSAGE_BEGIN 	'\x2'

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

bool ClientComm::send(std::string message)
{
	char messageBeginSignal = COMM_MESSAGE_BEGIN;
	uint32_t messageLenght = htonl(strlen(message.c_str()));
	/* Send message begin signal */
	if(write(this->socketFd, &messageBeginSignal, sizeof(messageBeginSignal)) < 0)
		return false;
	/* Send message lenght */
	if(write(this->socketFd, &messageLenght, sizeof(messageLenght)) < 0)
		return false;
	/* Send message payload */
	if(write(this->socketFd, message.c_str(), strlen(message.c_str())) < 0)
		return false;

	return true;
}

std::string ClientComm::receive(void)
{
	int n, remaining = 0;
	char messageSignal;
	uint32_t messageLenght;
	do {
		if(read(this->socketFd, &messageSignal, sizeof(messageSignal)) < 0){
			return std::string("");
		}
	} while(messageSignal != COMM_MESSAGE_BEGIN);
	if(read(this->socketFd, &messageLenght, sizeof(messageLenght)) < 0){
		return std::string("");
	}
	char* buffer = (char*) calloc(1, messageLenght);
	remaining = messageLenght;
	do {
		if((n = read(this->socketFd, &buffer[messageLenght - remaining], remaining)) < 0){
			return std::string("");
		}
		remaining -= n;
	} while(remaining != 0);
	return std::string(buffer);
}
