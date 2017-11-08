#include "file.hpp"
#include "action.hpp"
#include <libgen.h>
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

File::File(std::string path, std::string name){
	struct stat buffer;
	std::string fullPath = path + '/' + name;
	stat(fullPath.c_str(),&buffer);
	this->name = name;
	this->access = Timestamp(buffer.st_atim);
	this->modification = Timestamp(buffer.st_mtim);
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

std::map<std::string, std::string> File::parsePath(std::string pathName)
{
	char* bkp1 = (char*)pathName.c_str();
	char* bkp2 = (char*)pathName.c_str();

	std::map<std::string, std::string> data;

	data[ARG_FILENAME] = std::string(basename(bkp1));
	data[ARG_PATHNAME] = std::string(dirname(bkp2)) + "/";

	return data;
}

std::string File::toString(std::vector<File> files)
{
	std::string filesString = "";
	for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
	{
		filesString += (*it).toString() + "\n";
	}
	return filesString;
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
