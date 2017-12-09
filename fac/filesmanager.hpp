#ifndef __FILEMANAGER_HPP__
#define __FILEMANAGER_HPP__

#include <map>
#include <mutex>
#include <string>
#include "fileaccesscontrol.hpp"

class FilesManager
{
	private:
		std::mutex filesAccess;
		std::map<std::string, FileAccessControl*> files;

	public:
		FilesManager(void){};

		void P(std::string fileName, int type);
		void V(std::string fileName, int type);
};

#endif
