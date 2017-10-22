#include <iostream>
#include "clientComm.h"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	ClientComm client(atoi(argv[1])-1);
	client.connectServer("192.168.0.22", atoi(argv[1]));
	client.send("Hello from client");
	client.send("TESTE BUFFER SIZE   TESTE BUFFER SIZE   TESTE BUFFER SIZE   TESTE BUFFER SIZE   TESTE BUFFER SIZE   TESTE BUFFER SIZE");
	std::cout << client.receive() << "\n";
	client.receiveFile("client/test.txt");
	return 0;
}
