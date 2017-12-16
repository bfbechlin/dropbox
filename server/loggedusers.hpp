#ifndef __LOGGEDUSERS_HPP__
#define __LOGGEDUSERS_HPP__

#include <string>
#include <mutex>
#include "action.hpp"
#include "device.hpp"
#include "serveruser.hpp"

class LoggedUsers
{
	private:
		std::mutex userAcess;
		std::vector<ServerUser*> users;

	public:
		LoggedUsers(void){};

		void addUser(ServerUser* user);
		ServerUser* getUserByName(std::string userName);

		void removeDevice(ServerUser* user, Device* device);
		bool tryRemoveUser(ServerUser* user);

		void pushActionAll(Action action);
		std::vector<std::string> getUserNames(void);
		std::string toString(void);
};
#endif
