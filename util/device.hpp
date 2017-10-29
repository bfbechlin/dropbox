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
		std::queue<Action*> actions;

	public:
		Device(void);
		Device(ActiveProcess active, PassiveProcess passive, FolderManager *folder);
		ActiveProcess active;
		ActiveProcess passive;
};

#endif
