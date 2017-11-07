#include "serveruser.hpp"

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

void ServerUser::processResquest(Device* device)
{
	/* Blockable call */
	int actionType = device->nextActionResquest();
	device->processAction(actionType);
}

void ServerUser::executeAction(Device* device)
{
	/* Busy Waiting for actions */
	while(device->noAction());
	Action nextAction = device->popAction();

	device->executeAction(nextAction);
}

void ServerUser::notifyOthers(Device* device)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		/* Notify all except the caller */
		if((*it) != device)
			(*it)->pushAction(Action(ACTION_NOTIFY));
	}
}
