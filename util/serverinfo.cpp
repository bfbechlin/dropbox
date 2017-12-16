#include "serverinfo.hpp"
#include <iostream>
#include <string>


ServerInfo::ServerInfo(void) {
	this->ip = "";
	this->port = -1;
}

ServerInfo::ServerInfo(std::string encoded) {

	std::size_t pos;

	pos = encoded.find(":");
	this->ip = encoded.substr(0,pos);
	this->port =  std::atoi(encoded.substr(pos+1).c_str());
}

ServerInfo::ServerInfo(std::string ip, int port) {
	this->ip = ip;
	this->port = port;
}

std::string ServerInfo::getIp(void) {
	return this->ip;
}

int ServerInfo::getPort(void) {
	return this->port;
}

std::string ServerInfo::toString(void) {
	std::string buffer = this->ip == "" ? "0.0.0.0" : this->ip;
	return buffer + ":" + std::to_string(this->port);
}
