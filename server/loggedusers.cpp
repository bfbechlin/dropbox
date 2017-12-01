#include "loggedusers.hpp"
#include <iostream>

void LoggedUsers::addUser(ServerUser* user)
{
	std::unique_lock<std::mutex> lck(this->userAcess);
	this->users.push_back(user);
}

ServerUser* LoggedUsers::getUserByName(std::string userName)
{
	std::unique_lock<std::mutex> lck(this->userAcess);
	for (std::vector<ServerUser*>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		if((*it)->getName() == userName)
			return (*it);
	}
	return NULL;
}

void LoggedUsers::removeDevice(ServerUser* user, Device* device)
{
	std::unique_lock<std::mutex> lck(this->userAcess);
	user->removeDevice(device);
}

bool LoggedUsers::tryRemoveUser(ServerUser* user)
{
	std::unique_lock<std::mutex> lck(this->userAcess);
	if(user->noDevices()){
		for (std::vector<ServerUser*>::iterator it = this->users.begin(); it != this->users.end(); ++it)
		{
			if((*it) == user)
			{
				this->users.erase(it);
				delete user->folder;
				delete user;
				break;
			}
		}
		return true;
	}
	else
		return false;
}

std::string LoggedUsers::toString(void)
{
	std::string buffer("");
	std::unique_lock<std::mutex> lck(this->userAcess);
	for (std::vector<ServerUser*>::iterator it = this->users.begin(); it != this->users.end(); ++it)
	{
		buffer += "\t" + (*it)->toString() + "\n";
	}
	return buffer;
}
