#include "database.hpp"
#include "foldermanager.hpp"
#include "servercomm.hpp"
#include "communication.hpp"

#include <iostream>
#include <vector>
#include <thread>
#include "file.hpp"

int main(int argc, char* argv[])
{
	std::thread newUser();
	ServerComm server(atoi(argv[1]));
	if(!database){
		database = new Database(std::string(argv[2]));
	}
	std::cout << database->getPath() << "\n";
	while(1){
		ServerComm client1 = server.newConnection();
		Communication client(client1.getSocket());
		std::string userName = client.receiveMessage();
		std::cout << userName << std::endl;
		client.receiveFile("server/test.txt");
		FolderManager folder("./database/test");
		std::vector<File*> files = client.pull();
		for (std::vector<File*>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::cout << (*it)->toString() << "\n";
		}
	}

	return 0;
}
