#include "file.hpp"
#include <sys/stat.h>

File::File(){
	this->name = std::string("");
	this->access = Timestamp();
	this->modification = Timestamp();
}

File::File(std::string name){
	this->name = name;
	this->access = Timestamp();
	this->modification = Timestamp();
}

File::File(std::string name, Timestamp access, Timestamp mod)
{
	this->name = name;
	this->access = access;
	this->modification = mod;
}


std::string File::getName(void)
{
	return this->name;
}

Timestamp File::getAccess(void)
{
	return this->access;
}

Timestamp File::getModification(void)
{
	return this->modification;
}

std::string File::toString() const
{
	return std::string("  File: " + this->name +
		"\nAccess: " + this->access.toDate() +
		"\nModify: " + this->modification.toDate());
}



bool File::isTemp(std::string path)
{
    return (path.back() == '~');
}

bool File::isDir(std::string path)
{
    struct stat buffer;
	stat(path.c_str(), &buffer);
	return (S_ISDIR(buffer.st_mode) != 0);
}

bool File::isValid(std::string path)
{
    return !(isDir(path) || isTemp(path));
}

bool File::exists(std::string path)
{
	struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}
