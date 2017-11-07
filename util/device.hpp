#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include <queue>
#include "action.hpp"
#include "activeprocess.hpp"
#include "passiveprocess.hpp"
#include "foldermanager.hpp"

class Device
{
	private:
		std::queue<Action> actions;

	public:
		ActiveProcess active;
		PassiveProcess passive;

		Device(void){};
		Device(ActiveProcess active, PassiveProcess passive);

		/* Resquest for a action a blocks until it's resolved */
		void executeAction(Action action);
		/* Receive request for a action resolve this request */
		void processAction(int actionType);
		int nextActionResquest(void);

		void pushAction(Action newAction);
		Action popAction(void);
		bool noAction(void);


};

#endif
