#include <iostream>
#include "clientComm.h"

int main()
{
	ClientComm client(5001);
	client.connectServer("192.168.0.22", 5000);
	return 0;
}
