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

		void synchronize(void);
		void deleteFile(std::string fileName);
		void uploadFile(std::string path, std::string fileName);
		void downloadFile(std::string path, std::string fileName);

		void sendActionResquest(Action action);
};

#endif
