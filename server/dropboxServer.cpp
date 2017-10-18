#include <iostream>
#include "serverComm.h"

int main()
{
	int client;
	ServerComm server(5000);
	client = server.newConnection();
	return 0;
}
