#include "servercomm.hpp"
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


ServerComm::ServerComm(void)
{
	this->port = 0;


	initSSL();
	this->ctx = createContext();
	loadCertificates(this->ctx, "CertFile.pem", "KeyFile.pem");


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

ServerComm::ServerComm(int port)
{
	this->port = port;

	initSSL();
	this->ctx = createContext();
	loadCertificates(this->ctx, "CertFile.pem", "KeyFile.pem");

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

	if((listen(this->socketFd, 8)) != 0)
	{
		fprintf(stderr, "[server]~: ERROR on listing new connections.\n");
		exit(1);
	}
}

ServerComm::ServerComm(int port, int socketFd){
	this->port = port;
	this->socketFd = socketFd;
}


ServerComm::ServerComm(int port, int socketFd, SSL* ssl, SSL_CTX* ctx){
	this->port = port;
	this->socketFd = socketFd;
	this->ssl = ssl;
	this->ctx = ctx;
}

ServerComm* ServerComm::newConnection(void)
{
	int clientSocket;
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);

	if((clientSocket = accept(this->socketFd, (struct sockaddr *) &clientAddr, (socklen_t *) &clientAddrLen)) < 0)
	{
		fprintf(stderr, "[server]~: ERROR on accept connection from client.\n");
		exit(1);
	}

	fprintf(stderr, "[server]~: SUCCESS accepted connection with %s:%i\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

	this->ssl = SSL_new(this->ctx);
	SSL_set_fd(this->ssl, clientSocket);
	if(SSL_accept(this->ssl) == -1) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	return new ServerComm(this->port, clientSocket, this->ssl, this->ctx);
}

int ServerComm::isRoot(void) {
	return getuid() == 0 ? 1 : 0;
}
void ServerComm::initSSL(void) {
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
}
SSL_CTX* ServerComm::createContext(void) {
	const SSL_METHOD* method;
	SSL_CTX* ctx;

	method = SSLv23_server_method();
	ctx = SSL_CTX_new(method);
	if(!ctx) {
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}
void ServerComm::loadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile) {

	if(SSL_CTX_use_certificate_file(ctx,CertFile,SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	if(SSL_CTX_use_PrivateKey_file(ctx,KeyFile,SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		abort();
	}

	if( ! SSL_CTX_check_private_key(ctx)) {
		fprintf(stderr, "[server]~: Private key does not match the public certificate\n");
		abort();
	}
}
void ServerComm::showCertificates(SSL* ssl) {
	X509* cert;
	char* line;

	cert = SSL_get_peer_certificate(ssl);
	if(cert) {
		printf("[client]~: Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("[client]~: Subject: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("[client]~: Issuer: %s\n", line);
		free(line);
		X509_free(cert);
	} else {
		printf("[server]~: No certificates. \n");
	}

}
