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

void ClientUser::processResquest(void)
{
	if(this->folder != NULL){
		Action nextAction = this->device->nextActionResquest();
		//std::cout << "Procesing " << nextAction.getTypeName() << '\n';
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
