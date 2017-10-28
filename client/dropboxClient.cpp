#include <iostream>
#include "clientComm.hpp"

#include <stdlib.h>

int main(int argc, char* argv[])
{
	ClientComm client(atoi(argv[1])-1);
	client.connectServer("192.168.0.22", atoi(argv[1]));
	client.sendMessage("MAPLEX");
	client.sendFile("client/test.txt");
	return 0;
}
