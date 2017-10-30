#include "servercomm.hpp"
#include "communication.hpp"
#include "file.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
	ServerComm server(atoi(argv[1]));
	while(1){
		ServerComm client1 = server.newConnection();
		Communication client(client1.getSocket());
		std::string userName = client.receiveMessage();
		std::cout << userName << std::endl;
		std::cout << client.receiveMessage() << std::endl;
		client.receiveFile("server/test.txt");
		std::vector<File> files = client.pull();
		for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
		{
			std::cout << (*it).toString() << "\n";
		}
	}

	return 0;
}
