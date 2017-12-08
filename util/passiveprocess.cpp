#include "passiveprocess.hpp"
#include "folderdiff.hpp"
#include <unistd.h>

#include <iostream>

PassiveProcess::PassiveProcess(void)
{
	this->channel = NULL;
	this->folder = NULL;
}

PassiveProcess::PassiveProcess(Communication* channel, FolderManager *folder)
{
	this->channel = channel;
	this->folder = folder;
}

void PassiveProcess::setFolder(FolderManager* folder)
{
	this->folder = folder;
}

void PassiveProcess::synchronize(void)
{
	std::vector<File> files = this->folder->getFiles();
	this->channel->push(files);
}

void PassiveProcess::merge(void)
{
	std::vector<File> files = this->folder->getFiles();
	this->channel->push(files);
}

void PassiveProcess::deleteFile(ActionQueue* actions)
{
	std::string fileName = this->channel->receiveMessage();
	std::string pathName = this->folder->getPath() + fileName;
	if(File::exists(pathName) && File::isValid(pathName))
	{
		this->channel->sendMessage("OK");
		unlink(pathName.c_str());
		actions->pushFront(Action(ACTION_NOTIFY_ALL));
	}
	else
		this->channel->sendMessage("ERROR");
}

void PassiveProcess::uploadFile(ActionQueue* actions)
{
	std::string fileName = this->channel->receiveMessage();
	this->channel->receiveFile(this->folder->getPath() + fileName);
	this->channel->sendMessage("OK");
	actions->pushFront(Action(ACTION_NOTIFY_ALL));
}

void PassiveProcess::downloadFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	std::string pathName = this->folder->getPath() + fileName;

	if(File::exists(pathName) && File::isValid(pathName))
	{
		this->channel->sendMessage("OK");
		this->channel->sendFile(pathName);
	}
	else
		this->channel->sendMessage("ERROR");
}

void PassiveProcess::list(void)
{
	std::vector<File> files = this->folder->getFiles();
	this->channel->push(files);
}

int PassiveProcess::parseActionResquest(void)
{
	std::string message = this->channel->receiveMessage();
	if(message == std::string(""))
		//exit(0);
		return -1;
	return std::stoi(message);
}
