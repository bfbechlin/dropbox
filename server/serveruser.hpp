#ifndef __SERVERUSER_HPP__
#define __SERVERUSER_HPP__

#include <string>
#include <mutex>
#include "foldermanager.hpp"
#include "filesmanager.hpp"
#include "device.hpp"

class ServerUser
{
	private:
		std::mutex actionProcess;
		FilesManager fileAcess;
		std::string name;
		std::vector<Device*> devices;

	public:
		FolderManager* folder;

		ServerUser(void){};
		ServerUser(std::string name, FolderManager* folder);
		ServerUser(std::string name, FolderManager* folder, Device* device);

		std::string getName(void);
		FolderManager* getFolder(void);

		int newDevice(Device* device);
		void removeDevice(Device* device);
		Device* getDevice(int index);
		bool noDevices(void);

		void processResquest(Device* device);
		void executeAction(Device* device);
		void pushActionOthers(Device* device, Action action);
		void pushActionAll(Action action);

		std::string toString(void);
};

#endif
