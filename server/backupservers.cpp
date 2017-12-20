#include "backupservers.hpp"
#include <iostream>

void BackupServers::addServer(ServerInfo info, ServerComm* channel){
	this->servers.append(info);
	this->channels.push_back(channel);
}

void BackupServers::removeServer(ServerInfo info, ServerComm* channel){
	this->servers.remove(info);
	for (std::vector<ServerComm*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		if((*it) == channel) {
			this->channels.erase(it);
			break;
		}
	}
}

ServerInfo BackupServers::popServer(void){
	return this->servers.pop();
}

void BackupServers::setServers(std::string servers){
	this->servers.set(servers);
}

void BackupServers::atualizeAll(LoggedUsers* users){
	std::string servs = this->servers.encode();
	std::map<std::string, std::string> arguments;
	for (std::vector<ServerComm*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		(*it)->sendMessage("SET_SERVERS");
		(*it)->sendMessage(servs);
	}
	arguments[ARG_SERVERS] = servs;
	users->pushActionAll(Action(ACTION_BACKUPSERVERS, arguments));
}

void BackupServers::notifyAll(std::string userName){
	for (std::vector<ServerComm*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		(*it)->sendMessage("NEW_USER");
		(*it)->sendMessage(userName);
	}
}

std::string BackupServers::encode(void) {
	return this->servers.encode();
}

std::string BackupServers::toString(void) {
	return std::string(std::to_string(this->channels.size()) + this->servers.toString());
}
