#include "activeprocess.hpp"
#include "folderdiff.hpp"
#include "file.hpp"
#include <unistd.h>

#include <iostream>

ActiveProcess::ActiveProcess(void)
{
	this->channel = NULL;
	this->folder = NULL;
}

ActiveProcess::ActiveProcess(FolderManager *folder)
{
	this->channel = NULL;
	this->folder = folder;
}

ActiveProcess::ActiveProcess(Communication* channel, FolderManager *folder)
{
	this->channel = channel;
	this->folder = folder;
}

void ActiveProcess::setFolder(FolderManager* folder)
{
	this->folder = folder;
}

void ActiveProcess::synchronize(ActionQueue* actions)
{
	std::vector<Action> newActions;

	std::vector<File> remoteFiles = this->channel->pull();

	FolderDiff diff = this->folder->diff(remoteFiles);

	std::vector<File> create = diff.getCreatedFiles();
	std::vector<File> modified = diff.getModifiedFiles();
	std::vector<File> deleted = diff.getDeletedFiles();

	/* Remove deleted files */
	for (std::vector<File>::iterator it = deleted.begin(); it != deleted.end(); ++it)
	{
		std::map<std::string, std::string> args;
		args[ARG_FILENAME] = (*it).getName();
		newActions.push_back(Action(ACTION_SELF_DELETE, args));
	}

	std::vector<File> downloads = modified;
	/* Merge two vectors */
	downloads.insert(downloads.end(), create.begin(), create.end());
	for (std::vector<File>::iterator it = downloads.begin(); it != downloads.end(); ++it)
	{
		std::map<std::string, std::string> args;
		args[ARG_FILENAME] = (*it).getName();
		args[ARG_PATHNAME] = this->folder->getPath();
		newActions.push_back(Action(ACTION_DOWNLOAD, args));
	}

	/* Verify with some modification will be made */
	if ((create.size() + modified.size() + deleted.size()) != 0)
		newActions.push_back(Action(ACTION_NOTIFY_OTHERS));

	actions->pushFront(newActions);
}

void ActiveProcess::merge(ActionQueue* actions)
{
	std::vector<Action> newActions;
	std::vector<File> remoteFiles = this->channel->pull();

	FolderDiff diff = this->folder->diff(remoteFiles);

	std::vector<File> create = diff.getCreatedFiles();
	std::vector<File> modified = diff.getModifiedFiles();

	std::vector<File> downloads = modified;
	/* Merge two vectors */
	downloads.insert(downloads.end(), create.begin(), create.end());
	for (std::vector<File>::iterator it = downloads.begin(); it != downloads.end(); ++it)
	{
		std::map<std::string, std::string> args;
		args[ARG_FILENAME] = (*it).getName();
		args[ARG_PATHNAME] = this->folder->getPath();
		newActions.push_back(Action(ACTION_DOWNLOAD, args));
	}

	/* Verify with some modification will be made */
	if ((create.size() + modified.size()) != 0)
		newActions.push_back(Action(ACTION_NOTIFY_OTHERS));

	actions->pushFront(newActions);
}

void ActiveProcess::deleteFile(std::string fileName)
{
	this->channel->sendMessage(fileName);
	std::string stats = this->channel->receiveMessage();
	if(stats == "OK")
		this->buffer = std::string("File " + fileName + " was deleted successfull.\n");

	else
		this->buffer = std::string("Delete error, file " + fileName + " doesn't exists in remote files.\n");
}

void ActiveProcess::selfDeleteFile(std::string fileName)
{
	std::string pathName = this->folder->getPath() + fileName;
	if(File::exists(pathName) && File::isValid(pathName))
		unlink(pathName.c_str());
}

void ActiveProcess::uploadFile(std::string path, std::string fileName)
{
	this->channel->sendMessage(fileName);
	this->channel->sendFile(path + fileName);
	std::string stats = this->channel->receiveMessage();
	if(stats == "OK")
	{
		this->buffer = std::string("Upload of file " + fileName + " was successfull.\n");
	}
	else
		this->buffer = std::string("Upload error, file " + fileName + " doesn't uploaded to remote files.\n");
}

void ActiveProcess::downloadFile(std::string path, std::string fileName)
{
	this->channel->sendMessage(fileName);
	std::string stats = this->channel->receiveMessage();
	if(stats == "OK")
	{
		this->channel->receiveFile(path + fileName);
		this->buffer = std::string("Download of file " + fileName + " was successfull.\n");
	}
	else
		this->buffer = std::string("Download error, file " + fileName + " doesn't exists in remote files.\n");
}

void ActiveProcess::list(void)
{
	std::vector<File> remoteFiles = this->channel->pull();
	this->buffer = File::toString(remoteFiles);
}

void ActiveProcess::exit(void)
{
	/* Only stay for any response */
	this->channel->receiveMessage();
}

std::string ActiveProcess::getInfo(void)
{
	std::string buff = this->buffer;
	this->buffer = "";
	return buff;
}

void ActiveProcess::sendActionResquest(Action action)
{
	this->channel->sendMessage(action.encode());
}
