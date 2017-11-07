#ifndef __SERVERUSER_HPP__
#define __SERVERUSER_HPP__

#include <string>
#include "foldermanager.hpp"
#include "device.hpp"

class ServerUser
{
	private:
		std::string name;
		FolderManager* folder;
		std::vector<Device*> devices;

	public:
		ServerUser(void){};
		ServerUser(std::string name, FolderManager* folder);
		ServerUser(std::string name, FolderManager* folder, Device* device);

		std::string getName(void);
		FolderManager* getFolder(void);
		void newDevice(Device* device);
		void processResquest(Device* device);
		void executeAction(Device* device);
		void notifyOthers(Device* device);
};
#endif
