#include "database.hpp"
#include "foldermanager.hpp"
#include "servercomm.hpp"
#include "serveruser.hpp"
#include "device.hpp"

#include <iostream>
#include <iterator>
#include <vector>
#include <thread>

int userLogedIndex(std::vector<ServerUser*> users, std::string userName)
{
	int i = -1;
	for (std::vector<ServerUser*>::iterator it = users.begin(); it != users.end(); ++it)
	{
		i++;
		if((*it)->getName() == userName)
			return i;
	}
	return -1;
}

void activeThread(ServerUser* user, Device* device)
{
	while(1)
		user->executeAction(device);
}

void passiveThread(ServerUser* user, Device* device)
{
	while(1)
		user->processResquest(device);

}

int main(int argc, char* argv[])
{
	std::vector<ServerUser*> users;
	if(argc != 3 && argc != 2)
	{
		std::cout << "Usage:\n\t ./dropboxServer <PORT> <DATABASE>\n\tDefault database: ./database\n";
		exit(1);
	}

	ServerComm server(atoi(argv[1]));
	if(!database)
	{
		if(argc == 3)
			database = new Database(std::string(argv[2]));
		else
			database = new Database("./database");
	}

	std::cout << "[server]~: dropbox server is up, database: " << database->getPath() << "\n";
	while(1){
		std::cout << "[server]~: waiting for new users " << "\n";
		std::string userName;

		ServerComm* activeComm = server.newConnection();
		std::cout << "ACTIVE SERVER <-> " << activeComm->receiveMessage() << " CLIENT\n";
		ServerComm* passiveComm = server.newConnection();
		std::cout << "PASSIVE SERVER <-> " << passiveComm->receiveMessage() << " CLIENT\n";

		userName = passiveComm->receiveMessage();
		std::cout << "[server]~: user " << userName << " logged in.\n";
		ServerUser* thisUser;
		int index;
		if((index = userLogedIndex(users, userName)) == -1)
		{
			FolderManager* thisFolder = new FolderManager(std::string(
				database->getPath() + "sync_dir_" + userName
			));
			thisUser = new ServerUser(userName, thisFolder);
			users.push_back(thisUser);
		}
		else
		{
			thisUser = users[index];
		}
		FolderManager* thisFolder = thisUser->getFolder();
		Device* thisDevice = new Device(
			ActiveProcess(activeComm, thisFolder),
			PassiveProcess(passiveComm, thisFolder)
		);
		thisUser->newDevice(thisDevice);
		std::thread act(activeThread, thisUser, thisDevice);
		std::thread pass(passiveThread, thisUser, thisDevice);
		act.detach();
		pass.detach();
	}

	return 0;
}
