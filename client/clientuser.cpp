#include "clientuser.hpp"

#include <iostream>

ClientUser::ClientUser(std::string name, FolderManager* folder, Device* device)
{
	this->name = name;
	this->device = device;
	this->folder = folder;
	this->synchronized = false;
}

std::string ClientUser::getName(void)
{
	return this->name;
}

bool ClientUser::isSynchronized(void)
{
	std::unique_lock<std::mutex> lck(this->syncAcess);
	return this->synchronized;
}

void ClientUser::synchronize(void)
{
	std::unique_lock<std::mutex> lck(this->syncAcess);
	this->synchronized = true;
}

FolderManager* ClientUser::getFolder(void)
{
	return this->folder;
}

void ClientUser::setFolder(FolderManager* folder)
{
	this->folder = folder;
	if(this->device != NULL){
		this->device->active.setFolder(folder);
		this->device->passive.setFolder(folder);
	}
}

void ClientUser::setServer(ServerInfo server){
	this->primaryServer = server;
}

void ClientUser::processResquest(void)
{
	if(this->folder != NULL){
		Action nextAction = this->device->nextActionResquest();
		//std::cout << "Procesing " << nextAction.getTypeName() << '\n';
		if(nextAction.getType() == ACTION_INVALID) {

			this->device->setState(STATE_RECONNECTING);
		}
		else if(nextAction.getType() == ACTION_BACKUPSERVERS) {
			std::map<std::string, std::string> arguments = nextAction.getArguments();
			this->backupServers.set(arguments[ARG_SERVERS]);
		}
		this->device->processAction(nextAction);
	}
}

void ClientUser::executeAction(void)
{
	/* Busy Waiting for actions */
	while(this->device->actions.isEmpty());
	Action nextAction = this->device->actions.pop();
	//std::cout << "Executing " << nextAction.getTypeName() << '\n';
	this->device->executeAction(nextAction);

}

bool ClientUser::reconnect(void)
{
	bool connected = false;
	while(!connected){
		delete this->device->passive.channel;
		delete this->device->active.channel;

		ServerInfo server = this->backupServers.pop();
		if(server.getIp() == ""){
			return false;
		}
		ClientComm* passiveComm = new ClientComm();
		ClientComm* activeComm = new ClientComm();

		if(passiveComm->connectServer(server.getIp(), server.getPort())){
			passiveComm->sendMessage("CLIENT_ACTIVE");
			passiveComm->sendMessage(this->name);
			std::string code = passiveComm->receiveMessage();
			if(activeComm->connectServer(server.getIp(), server.getPort())){
				activeComm->sendMessage("CLIENT_PASSIVE");
				activeComm->sendMessage(this->name);
				activeComm->sendMessage(code);

				this->device->passive.channel = passiveComm;
				this->device->active.channel = activeComm;

				this->primaryServer = server;
				connected = true;
			}
		}
	}
	this->device->setState(STATE_RUNNING);
	return true;
}

std::string ClientUser::connectionInfo(void)
{
	return std::string("Primary:\n\t" + this->primaryServer.toString()
		+ "\nBackups:" + this->backupServers.toString());
}
