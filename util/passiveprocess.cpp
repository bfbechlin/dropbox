#include "passiveprocess.hpp"
#include "filediff.hpp"
#include <unistd.h>

void PassiveProcess::synchronize(std::map<std::string, std::string> arguments)
{
	std::vector<File> files = this->folder->getFiles();
	this->channel.push(files);
	int downloads = stoi(this->channel.receiveMessage());
	for(int i = 0; i < downloads; i++) {
		this->downloadFile(arguments);
	}
}

void PassiveProcess::deleteFile(std::map<std::string, std::string> arguments)
{
	std::string fileName = this->channel.receiveMessage();
	std::string path = arguments[ARG_SOURCEPATH] + fileName;
	unlink(path.c_str());
}

void PassiveProcess::uploadFile(std::map<std::string, std::string> arguments)
{
	std::string fileName = this->channel.receiveMessage();
	this->channel.receiveFile(arguments[ARG_DESTPATH] + fileName);
}

void PassiveProcess::downloadFile(std::map<std::string, std::string> arguments)
{
	std::string fileName = this->channel.receiveMessage();
	this->channel.sendFile(arguments[ARG_SOURCEPATH] + fileName);
}
