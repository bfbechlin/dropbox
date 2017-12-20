#include "clientcomm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

ClientComm::ClientComm(void)
{
	this->port = 0;

	initSSL();
	this->ctx = createContext();

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

	initSSL();
	this->ctx = createContext();

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
		return false;
	}

	this->ssl = SSL_new(this->ctx);
	SSL_set_fd(this->ssl, this->socketFd);

	if(SSL_connect(this->ssl) == -1) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	printf( "[client]~: Connected with %s encryption.\n", SSL_get_cipher(this->ssl));
	//showCertificates(this->ssl);

	return true;
}


void ClientComm::initSSL(void) {
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

}

SSL_CTX* ClientComm::createContext(void) {
	const SSL_METHOD* method;
	SSL_CTX* ctx;

	method = SSLv23_client_method();
	ctx = SSL_CTX_new(method);
	if(ctx == NULL) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

void ClientComm::showCertificates(SSL* ssl) {
	X509* cert;
	char* line;

	cert = SSL_get_peer_certificate(ssl);
	if(cert) {
		printf("[client]~: Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("[client]~: Subject: %s\n", line);
		free(line);       /* free the malloc'ed string */
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("[client]~: Issuer: %s\n", line);
		free(line);       /* free the malloc'ed string */
		X509_free(cert);     /* free the malloc'ed certificate copy */
	} else {
		printf("[client]~: No certificates. \n");
	}
	return;
}
