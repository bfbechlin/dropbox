#ifndef __SERVERLIST_HPP__
#define __SERVERLIST_HPP__

#include <mutex>
#include <vector>
#include <string>
#include "serverinfo.hpp"

class ServerList {
private:
	std::mutex acess;
  	std::vector<ServerInfo> servers;

public:
	  ServerList(void){};
	  ServerList(std::string servers);

	  int getSize(void);

	  ServerInfo pop(void);

	  void append(std::string server);
	  void append(ServerInfo server);

	  void set(std::string servers);
	  void set(std::vector<ServerInfo> servers);

	  void remove(ServerInfo server);

	  std::string encode(void);
	  std::vector<ServerInfo> decode(std::string servers);

	  std::string toString(void);

};




#endif
