#include "serveruser.hpp"
#include <sstream>
#include <iostream>

ServerUser::ServerUser(std::string name, FolderManager* folder)
{
	this->name = name;
	this->folder = folder;
	this->readers = 0;
}

ServerUser::ServerUser(std::string name, FolderManager* folder, Device* device)
{
	this->name = name;
	this->folder = folder;
	this->newDevice(device);
	this->readers = 0;
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

	std::unique_lock<std::mutex> lockReader (this->lReader,std::defer_lock);
	std::unique_lock<std::mutex> lockRW (this->lRW,std::defer_lock);

	switch (actionType) {
		// Write actions
		case ACTION_UPLOAD:
		case ACTION_DELETE:
			lockRW.lock();
			break;
		// Read action
		default:
			lockReader.lock();
			this->readers++;
			if(this->readers == 1)
				lockRW.lock();
			lockReader.unlock();
			break;
	}

	device->processAction(actionType);

	switch (actionType) {
		// Write actions
		case ACTION_UPLOAD:
		case ACTION_DELETE:
			lockRW.unlock();
			break;
		// Read action
		default:
			lockReader.lock();
			this->readers--;
			if(this->readers == 0)
				lockRW.unlock();
			lockReader.unlock();
			break;
	}

	/* Actions that modificate files */
	if(actionType == ACTION_UPLOAD)
		this->notifyOthers(device);

	if(actionType == ACTION_DELETE)
		this->notifyAll();
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

	std::unique_lock<std::mutex> lockReader (this->lReader,std::defer_lock);
	std::unique_lock<std::mutex> lockRW (this->lRW,std::defer_lock);
	switch (actionType) {
		// Write actions
		case ACTION_INITILIAZE:
		case ACTION_MERGE:
		case ACTION_SYNCHRONIZE:
		case ACTION_DOWNLOAD:
			lockRW.lock();
			break;
		// Read action
		default:
			lockReader.lock();
			this->readers++;
			if(this->readers == 1)
				lockRW.lock();
			lockReader.unlock();
			break;
	}

	device->executeAction(nextAction);

	switch (actionType) {
		// Write actions
		case ACTION_INITILIAZE:
		case ACTION_MERGE:
		case ACTION_SYNCHRONIZE:
		case ACTION_DOWNLOAD:
			lockRW.unlock();
			break;
		// Read action
		default:
			lockReader.lock();
			this->readers--;
			if(this->readers == 0)
				lockRW.unlock();
			lockReader.unlock();
			break;
	}
	/* Actions that modificate files */
	if(actionType == ACTION_SYNCHRONIZE)
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

void ServerUser::notifyAll(void)
{
	for (std::vector<Device*>::iterator it = this->devices.begin(); it != this->devices.end(); ++it)
		(*it)->pushAction(Action(ACTION_NOTIFY));
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
