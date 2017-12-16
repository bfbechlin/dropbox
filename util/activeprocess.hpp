#ifndef __ACTIVEPROCESS_HPP__
#define __ACTIVEPROCESS_HPP__

#include <string>
#include <vector>
#include "action.hpp"
#include "actionqueue.hpp"
#include "communication.hpp"
#include "foldermanager.hpp"

class ActiveProcess
{
	private:
		FolderManager* folder;
		std::string buffer;

	public:
		Communication* channel;

		ActiveProcess(void);
		ActiveProcess(FolderManager *folder);
		ActiveProcess(Communication* channel, FolderManager *folder);

		void setFolder(FolderManager* folder);

		void synchronize(ActionQueue* actions);
		void merge(ActionQueue* actions);
		void deleteFile(std::string fileName);
		void selfDeleteFile(std::string fileName);
		void uploadFile(std::string path, std::string fileName);
		void downloadFile(std::string path, std::string fileName);
		void list(void);
		void exit(void);

		std::string getInfo(void);
		void sendActionResquest(Action action);
};

#endif
