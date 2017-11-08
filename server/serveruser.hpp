#ifndef __SERVERUSER_HPP__
#define __SERVERUSER_HPP__

#include <string>
#include <mutex>
#include "foldermanager.hpp"
#include "device.hpp"

class ServerUser
{
	private:
		std::mutex lReader;
		std::mutex lRW;
		int readers;

		std::string name;
		std::vector<Device*> devices;

	public:
		FolderManager* folder;

		ServerUser(void){};
		ServerUser(std::string name, FolderManager* folder);
		ServerUser(std::string name, FolderManager* folder, Device* device);

		std::string getName(void);
		FolderManager* getFolder(void);

		void newDevice(Device* device);
		void removeDevice(Device* device);
		bool noDevices(void);

		void processResquest(Device* device);
		void executeAction(Device* device);
		void notifyOthers(Device* device);
		void notifyAll(void);

		std::string toString(void);
};
#endif
