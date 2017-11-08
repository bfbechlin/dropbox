#include "foldermanager.hpp"
#include "clientcomm.hpp"
#include "clientuser.hpp"
#include "device.hpp"
#include "file.hpp"

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
	user->device->pushAction(ACTION_EXIT);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	exit(0);
}

void ioThread(ClientUser* user, Device* device, FolderManager* folder)
{
	while(!device->isEndConnection())
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
			{
				argument = line;
			}
			else
			{
				argument = "";
			}
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
				device->pushAction(Action(ACTION_UPLOAD, args));
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
			device->pushAction(Action(ACTION_DOWNLOAD, args));
		}
		else if(command == "list_server"){
			device->pushAction(Action(ACTION_LIST));
		}
		else if(command == "list_client"){
			std::vector<File> files = folder->getFiles();
			std::cout << File::toString(files);
		}
		else if(command == "get_sync_dir"){

		}
		else if(command == "exit"){
			device->pushAction(Action(ACTION_EXIT));
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			exit(0);
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
	std::thread io(ioThread, user, thisDevice, thisFolder);
	act.join();
	pass.join();
	noti.join();
	io.join();

	delete user;
	delete thisDevice;
	delete thisFolder;
	delete activeComm;
	delete passiveComm;
	return 0;
}
