#include "fileaccesscontrol.hpp"

FileAccessControl::FileAccessControl(std::string fileName) {
	this->name = fileName;
	this->readers = 0;
	this->writers = 0;
}

void FileAccessControl::block(int type) {
	if(type == FILEACCESS_WRITE) {
		this->mutexWrite.lock();
		writers += 1;
		this->mutexWrite.unlock();
		this->rw.lock();
	}
	if(type == FILEACCESS_READ) {
		this->mutexRead.lock();
		readers += 1;
		if (readers == 1)
			this->rw.lock();
		this->mutexRead.unlock();
	}
}

void FileAccessControl::release(int type) {
	if(type == FILEACCESS_WRITE) {
		this->mutexWrite.lock();
		writers -= 1;
		this->mutexWrite.unlock();
		this->rw.unlock();
	}
	if(type == FILEACCESS_READ) {
		this->mutexRead.lock();
		readers -= 1;
		if (readers == 0)
			this->rw.unlock();
		this->mutexRead.unlock();
	}
}

bool FileAccessControl::noLock(void) {
	std::unique_lock<std::mutex> lck1(this->mutexWrite);
	std::unique_lock<std::mutex> lck2(this->mutexRead);
	return (readers == 0) && (writers == 0);
}
