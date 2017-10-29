#include <iostream>
#include "clientcomm.hpp"
#include "file.hpp"

int main(int argc, char* argv[])
{
	std::vector<File> files;
	ClientComm client;
	client.connectServer(std::string(argv[1]), atoi(argv[2]));
	client.sendMessage("MAPLEX");
	client.sendFile("client/test.txt");

	files.push_back(File("test.txt"));
	files.push_back(File("test2.txt"));
	client.push(files);

	return 0;
}
