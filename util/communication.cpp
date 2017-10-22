#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include <string>
#include <errno.h>

Communication::Communication(void)
{
	this->port = -1;
	this->socketFd = -1;
}

bool Communication::send(std::string message)
{
	char messageBeginSignal = COMM_MESSAGE_BEGIN;
	uint32_t messageLenght = htonl(strlen(message.c_str()));
	/* Send message begin signal */
	if(write(this->socketFd, &messageBeginSignal, sizeof(messageBeginSignal)) < sizeof(messageBeginSignal))
		return false;
	/* Send message lenght */
	if(write(this->socketFd, &messageLenght, sizeof(messageLenght)) < sizeof(messageLenght))
		return false;
	/* Send message payload */
	if(write(this->socketFd, message.c_str(), strlen(message.c_str())) < strlen(message.c_str()))
		return false;

	return true;
}

std::string Communication::receive(void)
{
	int readBytes, remainingBytes;
	char messageSignal;
	uint32_t messageLenght;
	do
	{
		if(read(this->socketFd, &messageSignal, sizeof(messageSignal)) < sizeof(messageSignal))
			return std::string("");
	} while(messageSignal != COMM_MESSAGE_BEGIN);
	if(read(this->socketFd, &messageLenght, sizeof(messageLenght)) < sizeof(messageLenght))
		return std::string("");

	messageLenght = ntohl(messageLenght);

	char* buffer = (char*) calloc(1, messageLenght);

	remainingBytes = messageLenght;
	do
	{
		if((readBytes = read(this->socketFd, &buffer[messageLenght - remainingBytes], remainingBytes)) < 0)
			return std::string("");
		remainingBytes -= readBytes;
	} while(remainingBytes != 0);
	return std::string(buffer);
}

bool Communication::sendFile(std::string fileSourcePath)
{
	int fd;
	char buffer[COMM_FILE_CHUNK_SIZE];
	int readBytes, sendBytes, remainingBytes;
	struct stat fileInfo;
	if((fd = open(fileSourcePath.c_str(), O_RDONLY)) < 0)
		return false;
	if(fstat(fd, &fileInfo) < 0)
	{
		close(fd);
		return false;
	}
	char fileBeginSignal = COMM_FILE_BEGIN;
	uint32_t fileSize = htonl(fileInfo.st_size);
	/* Send file begin signal */
	if(write(this->socketFd, &fileBeginSignal, sizeof(fileBeginSignal)) < sizeof(fileBeginSignal))
	{
		close(fd);
		return false;
	}
	/* Send file size */
	if(write(this->socketFd, &fileSize, sizeof(fileSize)) < sizeof(fileSize))
	{
		close(fd);
		return false;
	}
	/* Send file */
	remainingBytes = fileInfo.st_size;
	do
	{
		if((readBytes = read(fd, buffer, COMM_FILE_CHUNK_SIZE)) < 0)
		{
			close(fd);
			return false;
		}
		if(write(this->socketFd, buffer, readBytes) < readBytes)
		{
			close(fd);
			return false;
		}
		remainingBytes -= readBytes;
	} while(remainingBytes > 0);
	close(fd);
	return true;
}

bool Communication::receiveFile(std::string fileDestPath)
{
	int fd;
	char messageSignal;
	char buffer[COMM_FILE_CHUNK_SIZE];
	uint32_t fileSize;
	int readBytes, remainingBytes, intendedBytes;
	do
	{
		if(read(this->socketFd, &messageSignal, sizeof(messageSignal)) < 0)
			return false;
	} while(messageSignal != COMM_FILE_BEGIN);
	if(read(this->socketFd, &fileSize, sizeof(fileSize)) < 0)
		return false;

	fileSize = ntohl(fileSize);
	fd = open(fileDestPath.c_str(), O_WRONLY|O_CREAT, 0666);
	remainingBytes = fileSize;
	do
	{
		intendedBytes = remainingBytes > COMM_FILE_CHUNK_SIZE ?
			COMM_FILE_CHUNK_SIZE: remainingBytes;
		if((readBytes = read(this->socketFd, buffer, intendedBytes)) < 0)
		{
			close(fd);
			return false;
		}
		if(write(fd, buffer, readBytes) < readBytes)
		{
			close(fd);
			return false;
		}
		remainingBytes -= readBytes;
	} while(remainingBytes > 0);
	close(fd);
	return true;
}
