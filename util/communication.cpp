#include "communication.hpp"
#include "timestamp.hpp"

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
#include <sys/time.h>

#include <errno.h>
#include <iostream>

#include <openssl/ssl.h>
#include <openssl/err.h>


Communication::Communication(void)
{
	this->port = -1;
	this->socketFd = -1;
	this->ssl = NULL;
	this->ctx = NULL;
}

Communication::Communication(int socketFd)
{
	this->port = -1;
	this->socketFd = socketFd;
	this->ssl = NULL;
	this->ctx = NULL;
}

Communication::~Communication(void)
{
	close(this->socketFd);
}

int Communication::getSocket(void)
{
	return this->socketFd;
}

bool Communication::sendMessage(std::string message)
{
	char messageBeginSignal = COMM_MESSAGE_BEGIN;
	uint32_t messageLenght = strlen(message.c_str());
	messageLenght = htonl(messageLenght);

	if(SSL_write(this->ssl, &messageBeginSignal, sizeof(messageBeginSignal)) < (int)sizeof(messageBeginSignal))
		return false;

	if(SSL_write(this->ssl, &messageLenght, sizeof(messageLenght)) < (int)sizeof(messageLenght))
		return false;

	if(SSL_write(this->ssl, message.c_str(), strlen(message.c_str())) < (int)strlen(message.c_str()))
		return false;

	return true;
}


std::string Communication::receiveMessage(void)
{
	int readBytes, remainingBytes;
	char messageSignal;
	uint32_t messageLenght;

	do
	{
		if(SSL_read(this->ssl, &messageSignal, sizeof(messageSignal)) < (int)sizeof(messageSignal))
			return std::string(COMM_ERROR);
	} while(messageSignal != COMM_MESSAGE_BEGIN);
	if(SSL_read(this->ssl, &messageLenght, sizeof(messageLenght)) < (int)sizeof(messageLenght))
		return std::string(COMM_ERROR);

	messageLenght = ntohl(messageLenght);

	char* buffer = (char*) calloc(1, messageLenght);

	remainingBytes = messageLenght;
	do
	{
		if((readBytes = SSL_read(this->ssl, &buffer[messageLenght - remainingBytes], remainingBytes)) < 0)
			return std::string(COMM_ERROR);
		remainingBytes -= readBytes;
	} while(remainingBytes != 0);

	return std::string(buffer);
}


/*
	http://en.cppreference.com/w/c/chrono/timespec
	http://man7.org/linux/man-pages/man2/stat.2.html
	http://man7.org/linux/man-pages/man2/utime.2.html
*/

bool Communication::sendFile(std::string fileSourcePath)
{
	Timestamp modification, access;
	int fd;
	char buffer[COMM_FILE_CHUNK_SIZE];
	int readBytes, remainingBytes;
	struct stat fileInfo;
	uint32_t fileSize;
	struct tsencode timestampBuffer;
	char fileBeginSignal = COMM_FILE_BEGIN;

	if((fd = open(fileSourcePath.c_str(), O_RDONLY)) < 0)
		return false;
	if(fstat(fd, &fileInfo) < 0)
	{
		close(fd);
		return false;
	}

	modification = Timestamp(fileInfo.st_mtim);
	access = Timestamp(fileInfo.st_atim);
	fileSize = fileInfo.st_size;
	fileSize = htonl(fileSize);

	if(SSL_write(this->ssl, &fileBeginSignal, sizeof(fileBeginSignal)) < (int)sizeof(fileBeginSignal))
	{
		close(fd);
		return false;
	}
	if(SSL_write(this->ssl, &fileSize, sizeof(fileSize)) < (int)sizeof(fileSize))
	{
		close(fd);
		return false;
	}
	timestampBuffer = access.encode();
	if(SSL_write(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
	{
		close(fd);
		return false;
	}
	timestampBuffer = modification.encode();
	if(SSL_write(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
	{
		close(fd);
		return false;
	}
	remainingBytes = fileInfo.st_size;
	do
	{
		if((readBytes = read(fd, buffer, COMM_FILE_CHUNK_SIZE)) < 0)
		{
			close(fd);
			return false;
		}
		if(SSL_write(this->ssl, buffer, readBytes) < readBytes)
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
	Timestamp modification, access;
	struct tsencode timestampBuffer;
	struct timeval times[2];
	int readBytes = 0, remainingBytes, intendedBytes, writtenBytes;

	do
	{

		if(SSL_read(this->ssl, &messageSignal, sizeof(messageSignal)) < 0)
			return false;

	} while(messageSignal != COMM_FILE_BEGIN);

	if(SSL_read(this->ssl, &fileSize, sizeof(fileSize)) < 0)
		return false;
	fileSize = ntohl(fileSize);
	if(SSL_read(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
		return false;
	access = Timestamp(timestampBuffer);

	if(SSL_read(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
		return false;
	modification = Timestamp(timestampBuffer);

	unlink(fileDestPath.c_str());
	if((fd = open(fileDestPath.c_str(), O_WRONLY | O_CREAT, 0666)) < 0)
	{
		return false;
	}

	remainingBytes = fileSize;
	do
	{
		intendedBytes = remainingBytes > COMM_FILE_CHUNK_SIZE ?
			COMM_FILE_CHUNK_SIZE: remainingBytes;

		if(intendedBytes) readBytes = SSL_read(this->ssl, buffer, intendedBytes);

		if(readBytes < 0)
		{

			close(fd);
			return false;
		}

		if((writtenBytes = write(fd, buffer, readBytes)) < readBytes)
		{
			close(fd);
			return false;
		}

		remainingBytes -= readBytes;
	} while(remainingBytes > 0);

	close(fd);

	times[TIMEVAL_ACCESS] = access.toTimeval();
	times[TIMEVAL_MODIFICATION] = modification.toTimeval();
	utimes(fileDestPath.c_str(), times);

	return true;
}



bool Communication::push(std::vector<File> files)
{
	Timestamp modification, access;
	struct tsencode timestampBuffer;
	char pushBeginSignal = COMM_PUSH_BEGIN;

	if(SSL_write(this->ssl, &pushBeginSignal, sizeof(pushBeginSignal)) < (int)sizeof(pushBeginSignal))
		return false;

	this->sendMessage(std::to_string(files.size()));

	for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
	{
		access = (*it).getAccess();
		modification = (*it).getModification();
		this->sendMessage((*it).getName());

		timestampBuffer = access.encode();
		if(SSL_write(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
			return false;

		timestampBuffer = modification.encode();
		if(SSL_write(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
			return false;

	}

	return true;
}

std::vector<File> Communication::pull(void)
{
	std::vector<File> files;
	int lenght, i;
	char messageSignal;
	Timestamp modification, access;
	struct tsencode timestampBuffer;
	std::string fileName;

	do
	{
		if(SSL_read(this->ssl, &messageSignal, sizeof(messageSignal)) < 0)
			return files;
	} while(messageSignal != COMM_PUSH_BEGIN);


	lenght = std::stoi(this->receiveMessage());

	for(i = 0; i < lenght; i++)
	{
		fileName = this->receiveMessage();
		if(SSL_read(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
			break;

		access = Timestamp(timestampBuffer);

		if(SSL_read(this->ssl, &timestampBuffer, TIMESTAMP_LEN) < (int)TIMESTAMP_LEN)
			break;

		modification = Timestamp(timestampBuffer);

		files.push_back(File(fileName, access, modification));
	}

	return files;
}
