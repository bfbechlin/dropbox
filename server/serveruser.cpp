#include "serveruser.hpp"
#include <sstream>
#include <iostream>

ServerUser::ServerUser(std::string name, FolderManager* folder)
{
	this->name = name;
	this->folder = folder;
}

ServerUser::ServerUser(std::string name, FolderManager* folder, Device* device)
{
	this->name = name;
	this->folder = folder;
	this->newDevice(device);
}

std::string ServerUser::getName(void)
{
	return this->name;
}

FolderManager* ServerUser::getFolder(void)
{
	return this->folder;
}

void ServerUser::newDevice(Device* device)
{
	this->devices.push_back(device);
}

void ServerUser::removeDevice(Device* device)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		if((*it) == device){
			this->devices.erase(it);
			break;
		}
	}
}

bool ServerUser::noDevices(void)
{
	return this->devices.empty();
}

void ServerUser::processResquest(Device* device)
{
	/* Blockable call */
	int actionType = device->nextActionResquest();
	Action nextAction(actionType);
	std::cout << "["<< this->name << "@device" << static_cast<void*>(device) << "]~: Processing::" << nextAction.getTypeName() << "\n";

	device->processAction(actionType);

	/* Actions that modificate files */
	if(actionType == ACTION_UPLOAD || actionType == ACTION_DELETE)
		this->notifyOthers(device);
}

void ServerUser::executeAction(Device* device)
{
	/* Busy Waiting for actions */
	if(device->noAction()){
		return;
	}
	Action nextAction = device->popAction();
	int actionType = nextAction.getType();
	std::cout << "["<< this->name << "@device" << static_cast<void*>(device) << "]~: Executing::" << nextAction.getTypeName() << "\n";

	device->executeAction(nextAction);

	/* Actions that modificate files */
	if(actionType == ACTION_SYNCHRONIZE || actionType == ACTION_DOWNLOAD)
		this->notifyOthers(device);
}

void ServerUser::notifyOthers(Device* device)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		if((*it) != device){
			(*it)->pushAction(Action(ACTION_NOTIFY));
		}
	}
}

std::string ServerUser::toString(void)
{
	std::stringstream devs;
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		devs << "device" << static_cast<void*>((*it)) << " ";
	}

	return std::string(this->name ": " + devs.str());
}
