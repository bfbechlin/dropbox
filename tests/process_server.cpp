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
	std::string buffer;
	while(1){
		ServerComm* activeComm = server.newConnection();
		std::cout << activeComm->receiveMessage() << " ";
		std::cout << activeComm->receiveMessage() << "\n";
		ServerComm* passiveComm = server.newConnection();
		std::cout << passiveComm->receiveMessage() << " ";
		std::cout << passiveComm->receiveMessage() << "\n";

		ActiveProcess active(activeComm, &folder);
		PassiveProcess passive(passiveComm, &folder);

		active.channel->sendMessage("FROM ActiveProcess\n");
		std::cout << "-synchronizing\n";
		active.synchronize();
		std::cout << "write anything and enter to continue:\n";
		std::cin >> buffer;
		std::cout << "-deleting test.txt\n";
		passive.deleteFile();
		std::cout << "write anything and enter to continue:\n";
		std::cin >> buffer;
		std::cout << "-uploading test.txt\n";
		passive.uploadFile();
		std::cout << "End of routine, waiting for new connection\n";
		delete activeComm;
		delete passiveComm;
	}

	return 0;
}
