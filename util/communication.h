
/*
	Interface/Class for communication class with sockets
	ServerComm and ClientComm inherity by this class
*/

#include "../config/env.h"

namespace comm_protocol {
	enum messages {
		SEND_FILE, //...
	}
}


class Communication {
private:
	int socket_fd;
public:
	/* Constructor will be different to Server and Client */

};
