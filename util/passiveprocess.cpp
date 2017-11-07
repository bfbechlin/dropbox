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

void PassiveProcess::synchronize(void)
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
	std::string path = this->folder->getPath() + fileName;
	unlink(path.c_str());
}

void PassiveProcess::uploadFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	this->channel->receiveFile(this->folder->getPath() + fileName);
}

void PassiveProcess::downloadFile(void)
{
	std::string fileName = this->channel->receiveMessage();
	this->channel->sendFile(this->folder->getPath() + fileName);
}

int PassiveProcess::parseActionResquest(void)
{
	return std::stoi(this->channel->receiveMessage());
}
