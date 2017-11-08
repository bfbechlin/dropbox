#ifndef __ACTIVEPROCESS_HPP__
#define __ACTIVEPROCESS_HPP__

#include <string>
#include <vector>
#include "action.hpp"
#include "communication.hpp"
#include "foldermanager.hpp"

class ActiveProcess
{
	private:
		FolderManager* folder;
	public:
		Communication* channel;

		ActiveProcess(void);
		ActiveProcess(Communication* channel, FolderManager *folder);

		void setFolder(FolderManager* folder);

		void synchronize(void);
		void merge(void);
		std::string deleteFile(std::string fileName);
		void uploadFile(std::string path, std::string fileName);
		std::string downloadFile(std::string path, std::string fileName);
		std::string list(void);

		void sendActionResquest(Action action);
};

#endif
