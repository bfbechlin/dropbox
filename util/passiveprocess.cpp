#include "passiveprocess.hpp"
#include "filediff.hpp"
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
	int downloads = std::stoi(this->channel->receiveMessage());
	for(int i = 0; i < downloads; i++) {
		this->downloadFile();
	}
}

void PassiveProcess::merge(void)
{
	std::vector<File> files = this->folder->getFiles();
	this->channel->push(files);
	int downloads = std::stoi(this->channel->receiveMessage());
	for(int i = 0; i < downloads; i++) {
		this->downloadFile();
	}
}

void PassiveProcess::deleteFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	std::string pathName = this->folder->getPath() + fileName;
	if(File::exists(pathName) && File::isValid(pathName))
		this->channel->sendMessage("OK");
	else
		this->channel->sendMessage("ERROR");
	unlink(pathName.c_str());
}

void PassiveProcess::uploadFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	this->channel->receiveFile(this->folder->getPath() + fileName);
}

void PassiveProcess::downloadFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	std::string pathName = this->folder->getPath() + fileName;
	if(File::exists(pathName) && File::isValid(pathName))
		this->channel->sendMessage("OK");
	else
		this->channel->sendMessage("ERROR");

	this->channel->sendFile(this->folder->getPath() + fileName);
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
