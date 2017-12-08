#include "filesmanager.hpp"

void FilesManager::P(std::string fileName, int type) {

	std::map<std::string, FileAccessControl*>::iterator it;
	FileAccessControl* file;
	std::unique_lock<std::mutex> lck(this->filesAccess);
	it = this->files.find(fileName);
	if(it != this->files.end()) {
		file = this->files[fileName];
	} else {
		file = new FileAccessControl(fileName);
		this->files[fileName] = file;
	}
	file->block(type);
}

void FilesManager::V(std::string fileName, int type) {
	std::map<std::string, FileAccessControl*>::iterator it;

	std::unique_lock<std::mutex> lck(this->filesAccess);
	FileAccessControl* file = this->files[fileName];
	file->release(type);
	if(file->noLock()) {
		it = this->files.find(fileName);
		this->files.erase(it);
		delete file;
	}
}
