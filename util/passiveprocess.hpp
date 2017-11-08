#ifndef __PASSIVEPROCESS_HPP__
#define __PASSIVEPROCESS_HPP__

#include <string>
#include <vector>
#include "action.hpp"
#include "process.hpp"
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

		void synchronize(void);
		void merge(void);
		void deleteFile(void);
		void uploadFile(void);
		void downloadFile(void);
		void list(void);

		int parseActionResquest(void);
};

#endif
