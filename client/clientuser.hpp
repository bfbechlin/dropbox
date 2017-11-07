#ifndef __CLIENTUSER_HPP__
#define __CLIENTUSER_HPP__

#include <string>
#include "foldermanager.hpp"
#include "device.hpp"

class ClientUser
{
	private:
		std::string name;
		FolderManager* folder;


	public:
		Device* device;
		ClientUser(void){};
		ClientUser(std::string name, FolderManager* folder, Device* device);
		void processResquest(void);
		void executeAction(void);
};

#endif
