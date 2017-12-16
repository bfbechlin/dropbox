#ifndef __BACKUPSERVERS_HPP__
#define __BACKUPSERVERS_HPP__

#include "serverlist.hpp"
#include "loggedusers.hpp"
#include "servercomm.hpp"
#include "action.hpp"

class BackupServers {
private:
	ServerList servers;
	std::vector<ServerComm*> channels;

public:
	ServerInfo thisServer;

	BackupServers(void){};
	void addServer(ServerInfo info, ServerComm* channel);
	void removeServer(ServerInfo info, ServerComm* channel);
	ServerInfo popServer(void);
	void setServers(std::string servers);
	void atualizeAll(LoggedUsers* users);
	void notifyAll(std::string userName);
	std::string encode(void);
	std::string toString(void);
};

#endif
