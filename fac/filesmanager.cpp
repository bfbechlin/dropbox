#include "filesmanager.hpp"

#include <iostream>

void FilesManager::P(std::string fileName, int type) {

	std::map<std::string, FileAccessControl*>::iterator it;
	FileAccessControl* file;

	this->filesAccess.lock();
	it = this->files.find(fileName);
	if(it != this->files.end()) {
		file = this->files[fileName];
	} else {
		file = new FileAccessControl(fileName);
		this->files[fileName] = file;
	}
	this->filesAccess.unlock();

	file->block(type);
}

void FilesManager::V(std::string fileName, int type) {
	std::map<std::string, FileAccessControl*>::iterator it;

	this->filesAccess.lock();
	FileAccessControl* file = this->files[fileName];
	this->filesAccess.unlock();

	file->release(type);

	if(file->noLock()) {
		this->filesAccess.lock();
		it = this->files.find(fileName);
		this->files.erase(it);
		this->filesAccess.unlock();
		delete file;
	}
}
