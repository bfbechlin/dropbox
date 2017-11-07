#include "clientuser.hpp"

ClientUser::ClientUser(std::string name, FolderManager* folder, Device* device)
{
	this->name = name;
	this->device = device;
	this->folder = folder;
}

void ClientUser::processResquest(void)
{
	int actionType = this->device->nextActionResquest();
	this->device->processAction(actionType);
}

void ClientUser::executeAction(void)
{
	/* Busy Waiting for actions */
	while(this->device->noAction());
	Action nextAction = this->device->popAction();

	this->device->executeAction(nextAction);
}
