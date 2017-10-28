#include <iostream>
#include "clientComm.hpp"
#include "files.hpp"
#include <stdlib.h>

int main(int argc, char* argv[])
{
	std::vector<File*> files;
	ClientComm client(atoi(argv[1])-1);
	client.connectServer("192.168.0.25", atoi(argv[1]));
	client.sendMessage("MAPLEX");
	client.sendFile("client/test.txt");

	files.push_back(new File("test.txt"));
	files.push_back(new File("test2.txt"));
	//client.push(files);
	return 0;
}
