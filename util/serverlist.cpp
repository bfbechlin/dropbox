#include "serverlist.hpp"
#include <iostream>

#include "serverinfo.hpp"

ServerList::ServerList(std::string servers) {
	this->servers = this->decode(servers);
}


ServerInfo ServerList::pop(void) {
	if (this->servers.size() == 0) {
		return ServerInfo();
	} else {
		std::unique_lock<std::mutex> lck(this->acess);
		ServerInfo server = this->servers.at(0);
		this->servers.erase(this->servers.begin());
		return server;
	}
}

void ServerList::set(std::string servers) {
	 std::unique_lock<std::mutex> lck(this->acess);
	 this->servers = this->decode(servers);
}

void ServerList::set(std::vector<ServerInfo> servers) {
	 std::unique_lock<std::mutex> lck(this->acess);
	 this->servers = servers;
}

std::string ServerList::encode(void) {
  	std::string response;
	std::vector<ServerInfo> serversCopy;
	{
		std::unique_lock<std::mutex> lck(this->acess);
		serversCopy = this->servers;
	}
	for(unsigned i = 0; i < serversCopy.size(); i++) {
		serversCopy[i].getIp() == "" ? response += "0.0.0.0" : response += serversCopy[i].getIp();
		response += ":";
		response += std::to_string(serversCopy[i].getPort());
		response += "$";
	}

 	return response;
}

void ServerList::append(ServerInfo server) {
	bool found = false;
	std::unique_lock<std::mutex> lck(this->acess);
	for (std::vector<ServerInfo>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if(((*it).getIp() == server.getIp()) && ((*it).getPort()) == server.getPort()){
			found = true;
			break;
		}
	}
	if(!found)
		this->servers.push_back(server);
}

void ServerList::remove(ServerInfo server){
	std::unique_lock<std::mutex> lck(this->acess);
	for (std::vector<ServerInfo>::iterator it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		if(((*it).getIp() == server.getIp()) && ((*it).getPort()) == server.getPort()){
			this->servers.erase(it);
			break;
		}
	}
}

std::vector<ServerInfo> ServerList::decode(std::string servers) {

	std::vector<ServerInfo> response;
	std::string s = servers;
	std::string delimiter = "$";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		response.push_back(ServerInfo(token));
		s.erase(0, pos + delimiter.length());
	}

	return response;
}

std::string ServerList::toString(void) {

	std::string list;
	std::vector<ServerInfo> serversCopy;
	{
		std::unique_lock<std::mutex> lck(this->acess);
		serversCopy = this->servers;
	}
	for(unsigned i = 0; i < serversCopy.size(); i++) {
		list += "\n\t" + serversCopy.at(i).toString() ;
	}

	return list + "\n";
}
