#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include <queue>
#include <mutex>

#include "action.hpp"
#include "actionqueue.hpp"
#include "activeprocess.hpp"
#include "passiveprocess.hpp"
#include "foldermanager.hpp"

class Device
{
	private:
		std::mutex endAcess;
		bool endConn;

	public:
		ActionQueue actions;
		ActiveProcess active;
		PassiveProcess passive;

		Device(void){};
		Device(ActiveProcess active, PassiveProcess passive);

		std::string getMessage(void);

		void endConnection(void);
		bool isEndConnection(void);
		/* Resquest for a action a blocks until it's resolved */
		void executeAction(Action action);
		/* Receive request for a action resolve this request */
		void processAction(int actionType);
		int nextActionResquest(void);

		void sendFileName(std::string fileName);
		std::string receiveFileName();
};

#endif
