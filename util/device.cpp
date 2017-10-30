#include "device.hpp"

Device::Device(ActiveProcess active, PassiveProcess passive, FolderManager* folder)
{
	this->active = active;
	this->passive = passive;
	this->folder = folder;
}

void Device::pushAction(Action newAction)
{
	this->actions.push(newAction);
}

Action Device::popAction(void)
{
	if(!this->actions.empty())
	{
		Action action = this->actions.front();
		this->actions.pop();
		return action;
	}
	else
	{
		return Action();
	}
}

bool Device::noAction(void)
{
	return this->actions.empty();
}
