#include "database.hpp"
#include "foldermanager.hpp"
#include "servercomm.hpp"
#include "serveruser.hpp"
#include "device.hpp"


#include <iostream>
#include <iterator>
#include <vector>
#include <thread>

std::vector<ServerUser*> users;

void activeThread(ServerUser* user, Device* device)
{
	while(!device->isEndConnection())
		user->executeAction(device);

}

void passiveThread(ServerUser* user, Device* device)
{
	while(!device->isEndConnection()){
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		user->processResquest(device);
	}
}

void initThread(ServerUser* user, Device* device)
{
	std::thread act(activeThread, user, device);
	std::thread pass(passiveThread, user, device);

	act.join();
	pass.join();

	user->removeDevice(device);
	/* No user logged in this session with this user */
	if(user->noDevices())
	{
		for (std::vector<ServerUser*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			if((*it) == user)
			{
				std::cout << "[server]~: user " << (*it)->getName() << " has logout.\n";
				users.erase(it);
				delete user->folder;
				delete user;
				break;
			}
		}
	}
	std::cout << "[server]~: connected users:\n";
	for (std::vector<ServerUser*>::iterator it = users.begin(); it != users.end(); ++it)
	{
		std::cout << "\t" << (*it)->toString() << "\n";
	}
	delete device->active.channel;
	delete device->passive.channel;
	delete device;
}

int main(int argc, char* argv[])
{

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
		activeComm->receiveMessage();
		ServerComm* passiveComm = server.newConnection();
		passiveComm->receiveMessage();

		userName = passiveComm->receiveMessage();
		std::cout << "[server]~: user " << userName << " logged in.\n";

		ServerUser* thisUser;
		bool found = false;
		for (std::vector<ServerUser*>::iterator it = users.begin(); it != users.end(); ++it)
		{
			if((*it)->getName() == userName)
			{
				thisUser = (*it);
				found = true;
			}
		}
		if(!found){
			FolderManager* thisFolder = new FolderManager(std::string(
				database->getPath() + "sync_dir_" + userName
			));
			thisUser = new ServerUser(userName, thisFolder);
			users.push_back(thisUser);
		}
		FolderManager* thisFolder = thisUser->getFolder();
		Device* thisDevice = new Device(
			ActiveProcess(activeComm, thisFolder),
			PassiveProcess(passiveComm, thisFolder)
		);
		thisUser->newDevice(thisDevice);
		std::thread init(initThread, thisUser, thisDevice);
		init.detach();
	}

	return 0;
}
