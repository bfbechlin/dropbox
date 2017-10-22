#include <iostream>
#include "serverComm.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	ServerComm server(atoi(argv[1]));
	ServerComm client = server.newConnection();
	std::cout << client.receive() << "\n";
	std::cout << client.receive() << "\n";
	client.send("Hello from server");
	client.sendFile("server/test.txt");
	return 0;
}
