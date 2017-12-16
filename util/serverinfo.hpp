#ifndef __SERVERINFO_HPP__
#define __SERVERINFO_HPP__

#include <string>

class ServerInfo {
private:
	std::string ip;
	int port;

public:
	  ServerInfo(void);
	  ServerInfo(std::string encoded);
	  ServerInfo(std::string ip, int port);

	  std::string getIp(void);
	  int getPort(void);
	  std::string toString(void);
};


#endif
