#include "activeprocess.hpp"
#include "filediff.hpp"
#include <unistd.h>

#include <iostream>

ActiveProcess::ActiveProcess(void)
{
	this->channel = NULL;
	this->folder = NULL;
}

ActiveProcess::ActiveProcess(Communication* channel, FolderManager *folder)
{
	this->channel = channel;
	this->folder = folder;
}

void ActiveProcess::synchronize(void)
{
	std::vector<File> remoteFiles = this->channel->pull();

	FileDiff diff = this->folder->diff(remoteFiles);
	std::vector<File> create = diff.getCreatedFiles();
	std::vector<File> modified = diff.getModifiedFiles();
	std::vector<File> deleted = diff.getDeletedFiles();

	/* Remove deleted files */
	for (std::vector<File>::iterator it = deleted.begin(); it != deleted.end(); ++it)
	{
		std::string path = this->folder->getPath() + (*it).getName();
		unlink(path.c_str());
	}

	std::vector<File> downloads = modified;
	/* Merge two vectors */
	downloads.insert(downloads.end(), create.begin(), create.end());
	this->channel->sendMessage(std::to_string(downloads.size()));
	for (std::vector<File>::iterator it = downloads.begin(); it != downloads.end(); ++it)
	{
		this->downloadFile(this->folder->getPath(), (*it).getName());
	}
}

void ActiveProcess::deleteFile(std::string fileName)
{
	this->channel->sendMessage(fileName);
}

void ActiveProcess::uploadFile(std::string path, std::string fileName)
{
	this->channel->sendMessage(fileName);
	this->channel->sendFile(path + fileName);
}

void ActiveProcess::downloadFile(std::string path, std::string fileName)
{
	this->channel->sendMessage(fileName);
	this->channel->receiveFile(path + fileName);
}

void ActiveProcess::sendActionResquest(Action action)
{
	this->channel->sendMessage(std::to_string(action.getType()));
}
