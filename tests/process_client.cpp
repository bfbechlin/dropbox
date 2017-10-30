#include <map>
#include <iostream>
#include "clientcomm.hpp"
#include "communication.hpp"
#include "passiveprocess.hpp"
#include "activeprocess.hpp"
#include "foldermanager.hpp"
#include "action.hpp"

int main(int argc, char* argv[])
{
	ClientComm passiveComm, activeComm;
	std::map<std::string, std::string> args;
	FolderManager folder("./client");

	passiveComm.connectServer(std::string(argv[1]), atoi(argv[2]));
	passiveComm.sendMessage("maplex");
	passiveComm.sendMessage("passiveComm");

	activeComm.connectServer(std::string(argv[1]), atoi(argv[2]));
	activeComm.sendMessage("maplex");
	activeComm.sendMessage("activeComm");

	ActiveProcess active(&activeComm, &folder);
	PassiveProcess passive(&passiveComm, &folder);
	std::cout << "PASSIVEPROCESS::" << passive.channel->receiveMessage();

	args[ARG_PATHNAME] = "./client/";
	passive.synchronize(args);
	args[ARG_FILENAME] = "test.txt";
	active.deleteFile(args);
	active.uploadFile(args);

	return 0;
}
