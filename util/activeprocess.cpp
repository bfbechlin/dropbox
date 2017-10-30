#include "activeprocess.hpp"
#include "filediff.hpp"
#include <unistd.h>

void ActiveProcess::synchronize(std::map<std::string, std::string> arguments)
{
	std::vector<File> remoteFiles = this->channel->pull();

	FileDiff diff = this->folder->diff(remoteFiles);
	std::vector<File> create = diff.getCreatedFiles();
	std::vector<File> modified = diff.getModifiedFiles();
	std::vector<File> deleted = diff.getDeletedFiles();

	/* Remove deleted files */
	for (std::vector<File>::iterator it = deleted.begin(); it != deleted.end(); ++it)
	{
		std::string path = arguments[ARG_PATHNAME] + (*it).getName();
		unlink(path.c_str());
	}

	std::vector<File> downloads = modified;
	/* Merge two vectors */
	downloads.insert(downloads.end(), create.begin(), create.end());
	this->channel->sendMessage(std::to_string(downloads.size()));
	for (std::vector<File>::iterator it = downloads.begin(); it != downloads.end(); ++it)
	{
		std::map<std::string, std::string> args;
		args[ARG_FILENAME] = (*it).getName();
		args[ARG_PATHNAME] = arguments[ARG_PATHNAME];
		this->downloadFile(args);
	}
}

void ActiveProcess::deleteFile(std::map<std::string, std::string> arguments)
{
	this->channel->sendMessage(arguments[ARG_FILENAME]);
}

void ActiveProcess::uploadFile(std::map<std::string, std::string> arguments)
{
	this->channel->sendMessage(arguments[ARG_FILENAME]);
	this->channel->sendFile(arguments[ARG_PATHNAME] + arguments[ARG_FILENAME]);
}

void ActiveProcess::downloadFile(std::map<std::string, std::string> arguments)
{
	this->channel->sendMessage(arguments[ARG_FILENAME]);
	this->channel->receiveFile(arguments[ARG_PATHNAME] + arguments[ARG_FILENAME]);
}

void ActiveProcess::sendActionResquest(Action action)
{
	this->channel->sendMessage(action.getType());
}
