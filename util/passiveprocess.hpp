#ifndef __PASSIVEPROCESS_HPP__
#define __PASSIVEPROCESS_HPP__

#include <string>
#include <vector>
#include "action.hpp"
#include "actionqueue.hpp"
#include "communication.hpp"
#include "foldermanager.hpp"

class PassiveProcess
{
	private:
		FolderManager* folder;

	public:
		Communication* channel;

		PassiveProcess(void);
		PassiveProcess(Communication* channel, FolderManager *folder);

		void setFolder(FolderManager* folder);

		void synchronize(void);
		void merge(void);
		void deleteFile(ActionQueue* actions);
		void uploadFile(ActionQueue* actions);
		void downloadFile(void);
		void list(void);
		void exit(void);

		Action parseActionResquest(void);
};

#endif
