#include <map>
#include <iostream>
#include "servercomm.hpp"
#include "communication.hpp"
#include "passiveprocess.hpp"
#include "activeprocess.hpp"
#include "foldermanager.hpp"
#include "action.hpp"

int main(int argc, char* argv[])
{
	ServerComm server(atoi(argv[1]));
	FolderManager folder("./server");
	std::map<std::string, std::string> args;
	std::string buffer;
	while(1){
		ServerComm activeComm = server.newConnection();
		std::cout << activeComm.receiveMessage() << " ";
		std::cout << activeComm.receiveMessage() << "\n";
		ServerComm passiveComm = server.newConnection();
		std::cout << passiveComm.receiveMessage() << " ";
		std::cout << passiveComm.receiveMessage() << "\n";

		ActiveProcess active(&activeComm, &folder);
		PassiveProcess passive(&passiveComm, &folder);

		active.channel->sendMessage("FROM ActiveProcess\n");
		args[ARG_PATHNAME] = "./server/";
		std::cout << "-synchronizing\n";
		active.synchronize(args);
		std::cout << "write anything and enter to continue:\n";
		std::cin >> buffer;
		std::cout << "-deleting\n";
		passive.deleteFile(args);
		std::cout << "write anything and enter to continue:\n";
		std::cin >> buffer;
		std::cout << "-uploading\n";
		passive.uploadFile(args);
		std::cout << "End of routine, waiting for new connection\n";
	}

	return 0;
}
