#include <thread>
#include <iostream>
#include "serverComm.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	std::thread newUser();
	ServerComm server(atoi(argv[1]));
	while(1){
		ServerComm client = server.newConnection();
		std::string userName = client.receiveMessage();
		std::cout << userName << std::endl;
		client.receiveFile("server/test.txt");
	}

	return 0;
}
