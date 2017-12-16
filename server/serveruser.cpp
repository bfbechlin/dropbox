#include "serveruser.hpp"
#include <sstream>
#include <iostream>

std::mutex ioMutex;
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

int ServerUser::newDevice(Device* device)
{
	this->devices.push_back(device);
	return (this->devices.size() - 1);
}

Device* ServerUser::getDevice(int index)
{
	return this->devices[index];
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
	Action nextAction = device->nextActionResquest();
	int actionType = nextAction.getType();
	std::map<std::string, std::string> args = nextAction.getArguments();
	if(actionType == ACTION_INVALID){
		device->setState(STATE_CLOSING);
	}

	switch (actionType) {
		case ACTION_DELETE:
		case ACTION_UPLOAD:
			this->fileAcess.P(args[ARG_FILENAME], FILEACCESS_WRITE);
			break;
		case ACTION_DOWNLOAD:
			this->fileAcess.P(args[ARG_FILENAME], FILEACCESS_READ);
			break;
	}

	std::map<std::string, std::string>::iterator it = args.find(ARG_FILENAME);
	{
		std::unique_lock<std::mutex> lck(ioMutex);
		std::cout << "["<< this->name << "@device" << static_cast<void*>(device) << "]~: Processing :: " << nextAction.getTypeName()
		<< " " <<  (it != args.end() ? args[ARG_FILENAME] : "") << "\n";
	}
	device->processAction(actionType);

	switch (actionType) {
		case ACTION_DELETE:
		case ACTION_UPLOAD:
			this->fileAcess.V(args[ARG_FILENAME], FILEACCESS_WRITE);
			break;
		case ACTION_DOWNLOAD:
			this->fileAcess.V(args[ARG_FILENAME], FILEACCESS_READ);
			break;
	}
}

void ServerUser::executeAction(Device* device)
{
	/* Busy Waiting for actions */
	if(device->actions.isEmpty()){
		return;
	}
	Action nextAction = device->actions.pop();
	std::unique_lock<std::mutex> lck (this->actionProcess);
	int actionType = nextAction.getType();
	std::map<std::string, std::string> args = nextAction.getArguments();

	switch (actionType) {
		case ACTION_SELF_DELETE:
		case ACTION_DOWNLOAD:
			fileAcess.P(args[ARG_FILENAME], FILEACCESS_WRITE);
			break;
	}

	{
		std::unique_lock<std::mutex> lck(ioMutex);
		std::cout << "["<< this->name << "@device" << static_cast<void*>(device) << "]~: Executing  :: " << nextAction.getTypeName() << "\n";
		device->executeAction(nextAction);
	}

	switch (actionType) {
		case ACTION_SELF_DELETE:
		case ACTION_DOWNLOAD:
			fileAcess.V(args[ARG_FILENAME], FILEACCESS_WRITE);
			break;
		case ACTION_NOTIFY_OTHERS:
			this->pushActionAll(Action(ACTION_NOTIFY));
			break;
		case ACTION_NOTIFY_ALL:
			this->pushActionAll(Action(ACTION_NOTIFY));
			break;
	}


}

void ServerUser::pushActionOthers(Device* device, Action action)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		if((*it) != device){
			(*it)->actions.pushBack(action);
		}
	}
}

void ServerUser::pushActionAll(Action action)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
		(*it)->actions.pushBack(action);
}

std::string ServerUser::toString(void)
{
	std::stringstream devs;
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
	{
		devs << "device" << static_cast<void*>((*it)) << " ";
	}

	return std::string(this->name + ": " + devs.str());
}
