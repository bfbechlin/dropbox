#ifndef __SERVERCOMM_HPP__
#define __SERVERCOMM_HPP__

#include "../util/communication.hpp"
#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>

class ServerComm: public Communication
{
private:
	int isRoot(void);
	void initSSL(void);
	SSL_CTX* createContext(void);
	void loadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile);
	void showCertificates(SSL* ssl);

public:
	ServerComm(void);
	ServerComm(int port);
	ServerComm(int port, int socketFd);
	ServerComm(int port, int socketFd, SSL* ssl, SSL_CTX* ctx);
	ServerComm* newConnection(void);
};

#endif
