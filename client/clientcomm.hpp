#ifndef __CLIENTCOMM_HPP__
#define __CLIENTCOMM_HPP__

#include "../util/communication.hpp"
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

class ClientComm : public Communication
{
private:
	void initSSL(void);
	SSL_CTX* createContext(void);
	void showCertificates(SSL* ssl);

public:
	ClientComm(void);
	ClientComm(int port);
	bool connectServer(std::string serverIp, int serverPort);
};

#endif
