#include "foldermanager.hpp"
#include "clientcomm.hpp"
#include "clientuser.hpp"
#include "device.hpp"

#include <iostream>
#include <iterator>
#include <thread>
#include <chrono>
#include <csignal>

ClientUser* user;

void signalHandler( int signum ) {
	user->device->pushAction(ACTION_EXIT);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	exit(0);
}

void activeThread(ClientUser* user, Device* device)
{
	while(!device->isEndConnection())
		user->executeAction();
}

void passiveThread(ClientUser* user, Device* device)
{
	while(!device->isEndConnection())
		user->processResquest();
}

void notifyThread(ClientUser* user, Device* device, FolderManager* folder)
{
	while(!device->isEndConnection()){
		if(folder->isModified()){
			std::cout << "MODIFIED"<< "\n";
			user->device->pushAction(Action(ACTION_NOTIFY));
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(int argc, char* argv[])
{
	signal(SIGINT, signalHandler);
	if(argc != 5 && argc != 4)
	{
		std::cout << "Usage:\n\t ./dropboxServer <USERNAME> <ADRESS_SERVER> <PORT_SERVER> <SYNC_DYR>\n\tDefault sync_dir: /sync_dir_<USER>/\n";
		exit(1);
	}
	std::string userName(argv[1]);
	ClientComm* passiveComm = new ClientComm();
	ClientComm* activeComm = new ClientComm();

	passiveComm->connectServer(std::string(argv[2]), atoi(argv[3]));
	passiveComm->sendMessage("PASSIVE");
	activeComm->connectServer(std::string(argv[2]), atoi(argv[3]));
	activeComm->sendMessage("ACTIVE");

	activeComm->sendMessage(userName);

	std::string folderPath;
	if(argc == 5){
		folderPath = std::string(argv[4]);
	}
	else {
		folderPath = std::string("/sync_dir_" + userName);
	}

	FolderManager* thisFolder = new FolderManager(folderPath);
	Device* thisDevice = new Device(
		ActiveProcess(activeComm, thisFolder),
		PassiveProcess(passiveComm, thisFolder)
	);
	thisDevice->pushAction(Action(ACTION_INITILIAZE));
	user = new ClientUser(userName, thisFolder, thisDevice);

	std::thread act(activeThread, user, thisDevice);
	std::thread pass(passiveThread, user, thisDevice);
	std::thread noti(notifyThread, user, thisDevice, thisFolder);
	act.join();
	pass.join();
	noti.join();

	delete user;
	delete thisDevice;
	delete thisFolder;
	delete activeComm;
	delete passiveComm;
	return 0;
}
