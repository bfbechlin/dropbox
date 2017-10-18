
#include "communication.h"

Communication::Communication(){
	this.socketfd = socket(AF_INET, SOCK_STREAM, 0);
}
