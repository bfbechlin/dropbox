#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__

#include <map>
#include <string>
#include "communication.hpp"
#include "foldermanager.hpp"

class Process
{
	protected:
		FolderManager* folder;
	public:
		Communication* channel;
		Process(void)
		{
			this-> channel = NULL;
			this->folder = NULL;
		};

		Process(Communication* channel, FolderManager *folder)
		{
			this->channel = channel;
			this->folder = folder;
		};

		virtual void synchronize(std::map<std::string, std::string> arguments) = 0;
		virtual void deleteFile(std::map<std::string, std::string> arguments) = 0;
		virtual void uploadFile(std::map<std::string, std::string> arguments) = 0;
		virtual void downloadFile(std::map<std::string, std::string> arguments) = 0;
};

#endif
