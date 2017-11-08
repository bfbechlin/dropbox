#ifndef __CLIENTUSER_HPP__
#define __CLIENTUSER_HPP__

#include <string>
#include "foldermanager.hpp"
#include "device.hpp"

class ClientUser
{
	private:
		std::string name;
		bool synchronized;

	public:
		FolderManager* folder;

		Device* device;
		ClientUser(void){};
		ClientUser(std::string name, FolderManager* folder, Device* device);

		FolderManager* getFolder(void);
		void setFolder(FolderManager* folder);

		std::string getName(void);
		bool isSynchronized(void);
		void synchronize(void);
		void processResquest(void);
		void executeAction(void);
};

#endif
