#ifndef __ACTIVEPROCESS_HPP__
#define __ACTIVEPROCESS_HPP__

#include <string>
#include <vector>
#include "action.hpp"
#include "process.hpp"
#include "communication.hpp"
#include "foldermanager.hpp"

class ActiveProcess: public Process
{
	public:
		ActiveProcess(void)
		: Process(){};
		ActiveProcess(Communication channel, FolderManager *folder)
		: Process(channel, folder){};

		void synchronize(std::map<std::string, std::string> arguments);
		void deleteFile(std::map<std::string, std::string> arguments);
		void uploadFile(std::map<std::string, std::string> arguments);
		void downloadFile(std::map<std::string, std::string> arguments);
};

#endif
