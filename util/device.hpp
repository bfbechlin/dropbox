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
		FolderManager* folder;
		std::queue<Action> actions;

		ActiveProcess active;
		PassiveProcess passive;

	public:
		Device(){};
		Device(ActiveProcess active, PassiveProcess passive, FolderManager* folder);

		void pushAction(Action newAction);
		Action popAction(void);
		bool noAction(void);
};

#endif
