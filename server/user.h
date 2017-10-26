#ifndef __USER_H__
#define __USER_H__

#include <iostream>
#include <list>

class User
{
	private:
		std::vector<ServerComm> devices;
		std::string userId;
		std::map<std::string, FileInfo> files;

	public:
		void pull(ServerComm device);
		void push(ServerComm device);
		void receiveFile(ServerComm device);
		void sendFile(ServerComm device); 
}

#endif
