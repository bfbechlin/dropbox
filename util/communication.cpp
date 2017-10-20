#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

Communication::Communication(){
	this->port = -1;
	this->socketFd = -1;
}


bool Communication::send(std::string message)
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

std::string Communication::receive(void)
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

	messageLenght = ntohl(messageLenght);

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
