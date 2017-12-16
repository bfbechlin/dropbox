#include "backup.hpp"

void listenThread(ClientComm* comm, ServerInfo primaryServer, BackupServers* servers);
void initThread(ClientUser* user, Device* device);

void listenThread(ClientComm* comm, ServerInfo primaryServer, BackupServers* servers){
	bool backupServer = true;
	while(backupServer){
		std::string mode = comm->receiveMessage();
		if(mode == "SET_SERVERS") {
			servers->setServers(comm->receiveMessage());
		}
		else if(mode == "NEW_USER") {
			std::string userName = comm->receiveMessage();

			ClientComm* passiveComm = new ClientComm();
			ClientComm* activeComm = new ClientComm();
			if(!passiveComm->connectServer(primaryServer.getIp(), primaryServer.getPort()))
				exit(1);
			passiveComm->sendMessage("CLIENT_ACTIVE");
			passiveComm->sendMessage(userName);
			std::string code = passiveComm->receiveMessage();
			if(!activeComm->connectServer(primaryServer.getIp(), primaryServer.getPort()))
				exit(1);
			activeComm->sendMessage("CLIENT_PASSIVE");
			activeComm->sendMessage(userName);
			activeComm->sendMessage(code);

			FolderManager* thisFolder = new FolderManager(std::string(
				database->getPath() + "sync_dir_" + userName
			));
			Device* thisDevice = new Device(
				ActiveProcess(activeComm, thisFolder),
				PassiveProcess(passiveComm, thisFolder)
			);
			ClientUser* user = new ClientUser(userName, thisFolder, thisDevice);

			std::thread init(initThread, user, thisDevice);
			init.detach();
		}
		else if(mode == COMM_ERROR) {
			ServerInfo nextServer = servers->popServer();
			bool sameIP = nextServer.getIp() == servers->thisServer.getIp();
			bool samePort = nextServer.getPort() == servers->thisServer.getPort();
			if(sameIP && samePort){
				backupServer = false;
			} else {
				delete comm;
				backupSetup(nextServer, servers);
			}
		}
	}
}

void activeThread(ClientUser* user, Device* device) {
	while(device->getState() == STATE_RUNNING)
		user->executeAction();
}

void passiveThread(ClientUser* user, Device* device) {
	while(device->getState() == STATE_RUNNING)
		user->processResquest();
}

void initThread(ClientUser* user, Device* device) {
	std::thread active(activeThread, user, device);
	std::thread passive(passiveThread, user, device);
	active.join();
	passive.join();
	delete user->folder;
	delete user;
	delete device->active.channel;
	delete device->passive.channel;
	delete device;
}

void backupSetup(ServerInfo primaryServer, BackupServers* servers) {
	ClientComm* comm = new ClientComm();
	comm->connectServer(primaryServer.getIp(), primaryServer.getPort());
	comm->sendMessage("BACKUP_SERVER");
	comm->sendMessage(servers->thisServer.getIp());
	comm->sendMessage(std::to_string(servers->thisServer.getPort()));
	int len = std::stoi(comm->receiveMessage());
	for(int i = 0; i < len; i++){
		std::string userName = comm->receiveMessage();

		ClientComm* passiveComm = new ClientComm();
		ClientComm* activeComm = new ClientComm();
		if(!passiveComm->connectServer(primaryServer.getIp(), primaryServer.getPort()))
			exit(1);
		passiveComm->sendMessage("CLIENT_ACTIVE");
		passiveComm->sendMessage(userName);
		std::string code = passiveComm->receiveMessage();
		if(!activeComm->connectServer(primaryServer.getIp(), primaryServer.getPort()))
			exit(1);
		activeComm->sendMessage("CLIENT_PASSIVE");
		activeComm->sendMessage(userName);
		activeComm->sendMessage(code);

		FolderManager* thisFolder = new FolderManager(std::string(
			database->getPath() + "sync_dir_" + userName
		));
		Device* thisDevice = new Device(
			ActiveProcess(activeComm, thisFolder),
			PassiveProcess(passiveComm, thisFolder)
		);
		ClientUser* user = new ClientUser(userName, thisFolder, thisDevice);

		std::thread init(initThread, user, thisDevice);
		init.detach();
	}
	std::thread listen(listenThread, comm, primaryServer, servers);
	listen.detach();
}
