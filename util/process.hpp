#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__

#include <map>
#include <string>
#include "communication.hpp"
#include "foldermanager.hpp"

class Process
{
	protected:
		Communication channel;
		FolderManager* folder;
	public:
		Process(void)
		{
			this-> channel = Communication();
			this->folder = NULL;
		};

		Process(Communication channel, FolderManager *folder)
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
