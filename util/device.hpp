#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include <queue>
#include <mutex>

#include "action.hpp"
#include "actionqueue.hpp"
#include "activeprocess.hpp"
#include "passiveprocess.hpp"
#include "foldermanager.hpp"

#define STATE_RUNNING 0
#define STATE_CLOSING 1
#define STATE_RECONNECTING 2

class Device
{
	private:
		std::mutex stateAcess;
		int state;

	public:
		ActionQueue actions;

		ActiveProcess active;
		PassiveProcess passive;

		Device(void){};
		Device(ActiveProcess active, PassiveProcess passive);

		std::string getMessage(void);

		void endConnection(void);
		int getState(void);
		void setState(int state);
		/* Resquest for a action a blocks until it's resolved */
		void executeAction(Action action);
		/* Receive request for a action resolve this request */
		void processAction(Action action);
		Action nextActionResquest(void);
};

#endif
