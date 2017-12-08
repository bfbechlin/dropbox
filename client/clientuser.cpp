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
		int actionType = this->device->nextActionResquest();
		Action nextAction(actionType);
		std::cout << "Processing " << nextAction.getTypeName() << '\n';
		this->device->processAction(actionType);
		std::cout << "Processing " << nextAction.getTypeName() << '\n';
	}
}

void ClientUser::executeAction(void)
{
	/* Busy Waiting for actions */
	while(this->device->actions.isEmpty());
	std::string fileName;
	Action nextAction = this->device->actions.pop();
	std::map<std::string, std::string> args = nextAction.getArguments();
	std::map<std::string, std::string>::iterator it = args.find(ARG_FILENAME);
	if(it != args.end()) {
		fileName = args[ARG_FILENAME];
	} else {
		fileName = "No File";
	}
	this->device->sendFileName(fileName);
	std::cout << "Executing " << nextAction.getTypeName() << " " <<  fileName << '\n';
	this->device->executeAction(nextAction);
	std::cout << "Executing " << nextAction.getTypeName() << " " << fileName <<'\n';
}
