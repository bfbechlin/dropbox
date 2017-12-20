#include "foldermanager.hpp"
#include "clientcomm.hpp"
#include "clientuser.hpp"
#include "device.hpp"
#include "file.hpp"
#include "serverinfo.hpp"

#include <iostream>
#include <iterator>
#include <thread>
#include <chrono>
#include <map>

#include <csignal>
#include <condition_variable>
#include <mutex>

ClientUser* user;

void signalHandler( int signum ) {
	user->device->setState(STATE_CLOSING);
	user->device->executeAction(Action(ACTION_EXIT));
	exit(0);
}

void ioThread(Device* device)
{
	std::mutex block;
	std::condition_variable response;
	while(device->getState() != STATE_CLOSING){
		while(device->getState() == STATE_RUNNING)
		{
			std::string line;
			std::string command;
			std::string argument;
			std::size_t pos;

	        std::cout << "[" << user->getName() << "@dropbox]~: ";
	        std::getline(std::cin, line);

			if((pos = line.find(" ")) != std::string::npos)
			{
				command = line.substr(0, pos);
				line = line.substr(pos+1, std::string::npos);
				if(line != "")
					argument = line;
				else
					argument = "";
				line = "";
			}
			else
			{
				command = line;
				line = "";
			}

			if(command == "upload"){
				if(argument == ""){
					std::cout << "\tCommand 'upload' needs argument. Type 'help' for more information.\n";
				}
				if(File::exists(argument) && File::isValid(argument)){
					std::map<std::string, std::string> args = File::parsePath(argument);
					std::unique_lock<std::mutex> lck(block);
					device->actions.pushBack(Action(ACTION_UPLOAD, args, &response));
					response.wait(lck);
					std::cout << device->getMessage();
				} else {
					std::cout << "\t File "<< argument <<" not exists.\n";
				}
			}
			else if(command == "download"){
				std::map<std::string, std::string> args;
				if(argument == ""){
					std::cout << "\tCommand 'download' needs argument. Type 'help' for more information.\n";
				}
				args[ARG_FILENAME] = argument;
				args[ARG_PATHNAME] = "./";
				std::unique_lock<std::mutex> lck(block);
				device->actions.pushBack(Action(ACTION_DOWNLOAD, args, &response));
				response.wait(lck);
				std::cout << device->getMessage();
			}
			else if(command == "delete"){
				std::map<std::string, std::string> args;
				if(argument == ""){
					std::cout << "\tCommand 'delete' needs argument. Type 'help' for more information.\n";
				}
				args[ARG_FILENAME] = argument;
				std::unique_lock<std::mutex> lck(block);
				device->actions.pushBack(Action(ACTION_DELETE, args, &response));
				response.wait(lck);
				std::cout << device->getMessage();
			}
			else if(command == "list_server"){
				std::unique_lock<std::mutex> lck(block);
				device->actions.pushBack(Action(ACTION_LIST, &response));
				response.wait(lck);
				std::cout << device->getMessage();
			}
			else if(command == "list_client"){
				if(!user->isSynchronized()){
					std::cout << "\tUser not synchronized with remote server.\n";
				} else {
					std::vector<File> files = user->folder->getFiles();
					std::cout << File::toString(files);
				}
			}
			else if(command == "get_sync_dir"){
				if(!user->isSynchronized()){
					FolderManager* folder;
					if(argument != ""){
						folder = new FolderManager(argument);
						if(!File::isDir(argument)){
							std::cout << "\tDirectory " << argument <<" not exists and cannot be created .\n";
							delete folder;
						} else {
							user->setFolder(folder);
							user->synchronize();
							device->actions.pushBack(Action(ACTION_MERGE));
						}
					} else {
						std::string path = "./sync_dir_" + user->getName();
						folder = new FolderManager(path);
						if(!File::isDir(path)){
							std::cout << "\tDirectory" << path <<" not exists and cannot be created .\n";
							delete folder;
						} else {
							user->setFolder(folder);
							user->synchronize();
							device->actions.pushBack(Action(ACTION_MERGE));
						}
					}
				} else {
					std::cout << "\tAlready synchronized at'" << user->folder->getPath() << "'.\n";
				}
			}
			else if(command == "connection_info"){
				std::cout << user->connectionInfo();
			}
			else if(command == "exit"){
				signalHandler(0);
			}
			else if(command == "help"){
				std::cout << "Commands: ";
			    std::cout << "\n\tupload <path/fileName.ext> ";
			    std::cout << "\n\tdownload <fileName.ext> ";
			    std::cout << "\n\tlist_server";
			    std::cout << "\n\tlist_client";
			    std::cout << "\n\tget_sync_dir";
			    std::cout << "\n\thelp";
			    std::cout << "\n\texit\n";
			}
			else {
				std::cout << "\tCommand '" << command << "' not found. Type 'help' for more information.\n";
			}
		}
	}
}

void activeThread(Device* device)
{
	while(device->getState() != STATE_CLOSING){
		while(device->getState() == STATE_RUNNING)
			user->executeAction();
	}
}

void passiveThread(Device* device)
{
	while(device->getState() != STATE_CLOSING){
		while(device->getState() == STATE_RUNNING){
			user->processResquest();
		}
		if(device->getState() == STATE_RECONNECTING){
			if(!user->reconnect()){
				std::cout << "\n[" << user->getName() << "@dropbox]~: CONNECTION_REFUSED\n" ;
				exit(1);
			}
		}
	}
}

void notifyThread(Device* device, FolderManager* folder)
{
	while(device->getState() == STATE_RUNNING){
		if(user->isSynchronized()){
			if(user->folder->isModified()){
				user->device->actions.pushBack(Action(ACTION_NOTIFY));
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main(int argc, char* argv[])
{
	signal(SIGINT, signalHandler);
	if(argc != 5 && argc != 4)
	{
		std::cout << "Usage:\n\t ./dropboxServer <USERNAME> <ADRESS_SERVER> <PORT_SERVER> [SYNC_DIR]\n";
		exit(1);
	}

	FolderManager* thisFolder;
	if(argc == 5){
		thisFolder = new FolderManager(std::string(argv[4]));
	}
	else {
		thisFolder = NULL;
	}

	std::string userName(argv[1]);
	ClientComm* passiveComm = new ClientComm();
	ClientComm* activeComm = new ClientComm();

	if(!passiveComm->connectServer(std::string(argv[2]), atoi(argv[3])))
		exit(1);
	passiveComm->sendMessage("CLIENT_ACTIVE");
	passiveComm->sendMessage(userName);
	std::string code = passiveComm->receiveMessage();
	if(!activeComm->connectServer(std::string(argv[2]), atoi(argv[3])))
		exit(1);
	activeComm->sendMessage("CLIENT_PASSIVE");
	activeComm->sendMessage(userName);
	activeComm->sendMessage(code);


	Device* thisDevice = new Device(
		ActiveProcess(activeComm, thisFolder),
		PassiveProcess(passiveComm, thisFolder)
	);

	user = new ClientUser(userName, thisFolder, thisDevice);
	if(argc == 5) {
		user->synchronize();
		thisDevice->actions.pushBack(Action(ACTION_MERGE));
	}

	user->setServer(ServerInfo(std::string(argv[2]), atoi(argv[3])));

	std::thread io(ioThread, thisDevice);
	std::thread act(activeThread, thisDevice);
	std::thread pass(passiveThread, thisDevice);
	std::thread noti(notifyThread, thisDevice, thisFolder);

	io.join();

	return 0;
}
