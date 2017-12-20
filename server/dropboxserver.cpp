#include "database.hpp"
#include "action.hpp"
#include "foldermanager.hpp"
#include "servercomm.hpp"
#include "serveruser.hpp"
#include "loggedusers.hpp"
#include "device.hpp"
#include "backupservers.hpp"
#include "serverinfo.hpp"
#include "backup.hpp"


#include <iostream>
#include <iterator>
#include <vector>
#include <thread>

LoggedUsers users;
BackupServers backupServers;

void activeThread(ServerUser* user, Device* device)
{
	while(device->getState() == STATE_RUNNING){
		user->executeAction(device);
	}
}

void passiveThread(ServerUser* user, Device* device)
{
	while(device->getState() == STATE_RUNNING){
		user->processResquest(device);
	}
}

void initThread(ServerUser* user, Device* device)
{
	std::map<std::string, std::string> arguments;
	arguments[ARG_SERVERS] = backupServers.encode();
	device->actions.pushBack(Action(ACTION_BACKUPSERVERS, arguments));
	device->actions.pushBack(Action(ACTION_MERGE));
	std::cout << "["<< user->getName() << "@device" << static_cast<void*>(device) << "]~: New device connected!!!" << "\n";
	std::thread act(activeThread, user, device);
	std::thread pass(passiveThread, user, device);

	act.join();
	pass.join();

	users.removeDevice(user, device);
	/* If no device logged in this session with this user, remove it */
	std::string userName = user->getName();
	if(users.tryRemoveUser(user))
		std::cout << "[server]~: user " << userName << " has logout.\n";

	std::cout << "[server]~: connected users:\n" << users.toString();
	delete device->active.channel;
	delete device->passive.channel;
	delete device;
}

void backupHealthCheck(ServerComm* comm, ServerInfo backup)
{
	while(1)
	{
		std::string healthCheck = comm->receiveMessage();
		if(healthCheck == COMM_ERROR){
			backupServers.removeServer(backup, comm);
			backupServers.atualizeAll(&users);
			break;
		}
	}
	delete comm;
	std::cout << "[server]~: backup server disconnect" << "\n";
	std::cout << "[server]~: connected backups: " << backupServers.toString();
}

int main(int argc, char* argv[])
{

	if(argc != 3 && argc != 5 && argc != 6 && argc != 7 && argc != 8)
	{
		std::cout << "Usage:\t ./dropboxServer <IP> <PORT> [-d <PATH>] [-b <IP> <PORT>]";
		std::cout << "\n\nOptinal parameters:";
		std::cout << "\n   -d: database path. DEFAULT: ./database";
		std::cout << "\n   -b: backup server, IP and PORT refer to primary server.";
		std::cout << "\n";
		exit(1);
	}

	ServerComm server(atoi(argv[2]));
	backupServers.thisServer = ServerInfo(std::string(argv[1]), std::stoi(argv[2]));

	/* Search for -d parameter */
	std::string databasePath("./database");
	for(int i = 0; i < argc; i ++){
		if(std::string(argv[i]) == "-d")
			databasePath = std::string(argv[i+1]);
	}

	if(!database)
		database = new Database(databasePath);

	/* Search for -b parameter */
	for(int i = 0; i < argc; i ++){
		if(std::string(argv[i]) == "-b"){
			ServerInfo primaryServer(std::string(argv[i+1]), std::stoi(argv[i+2]));
			backupSetup(primaryServer, &backupServers);
		}
	}

	std::cout << "[server]~: dropbox server is up at " << argv[1] << ":" << argv[2] <<", database: " << database->getPath() << "\n";
	while(1){

		ServerComm* newComm = server.newConnection();
		std::string mode = newComm->receiveMessage();

		if(mode == "CLIENT_ACTIVE"){
			std::string userName = newComm->receiveMessage();

			ServerUser* thisUser = users.getUserByName(userName);
			if(thisUser == NULL)
			{
				FolderManager* thisFolder = new FolderManager(std::string(
					database->getPath() + "sync_dir_" + userName
				));
				thisUser = new ServerUser(userName, thisFolder);
				users.addUser(thisUser);
				backupServers.notifyAll(userName);
				std::cout << "[server]~: user " << userName << " logged in.\n";
			}
			FolderManager* thisFolder = thisUser->getFolder();
			Device* thisDevice = new Device(
				ActiveProcess(newComm, thisFolder),
				PassiveProcess(thisFolder)
			);
			int index = thisUser->newDevice(thisDevice);
			newComm->sendMessage(std::to_string(index));
		}
		else if(mode == "CLIENT_PASSIVE") {
			std::string userName = newComm->receiveMessage();
			int index = std::stoi(newComm->receiveMessage());
			ServerUser* thisUser = users.getUserByName(userName);
			if(thisUser != NULL)
			{
				Device* thisDevice = thisUser->getDevice(index);
				thisDevice->passive.channel = newComm;
				std::thread init(initThread, thisUser, thisDevice);
				init.detach();
			}
		}
		else if(mode == "BACKUP_SERVER") {
			std::string ip = newComm->receiveMessage();
			int port = std::stoi(newComm->receiveMessage());
			ServerInfo backup(ip, port);
			std::vector<std::string> userNames = users.getUserNames();
			newComm->sendMessage(std::to_string(userNames.size()));
			for (std::vector<std::string>::iterator it = userNames.begin(); it != userNames.end(); ++it)
			{
				newComm->sendMessage((*it));
			}

			backupServers.addServer(backup, newComm);
			backupServers.atualizeAll(&users);
			std::thread healthCheck(backupHealthCheck, newComm, backup);
			healthCheck.detach();
			std::cout << "[server]~: backup server connected" << "\n";
			std::cout << "[server]~: connected backups: " << backupServers.toString();
		}
	}

	return 0;
}
