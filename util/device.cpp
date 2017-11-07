#include "device.hpp"

Device::Device(ActiveProcess active, PassiveProcess passive)
{
	this->active = active;
	this->passive = passive;
}

void Device::executeAction(Action action)
{
	std::map<std::string, std::string> args = action.getArguments();
	int actionType = action.getType();

	/* Only active channel used*/
	this->active.sendActionResquest(action);
	switch (actionType)
	{
		case ACTION_INITILIAZE:
			this->active.synchronize();
			break;
		case ACTION_SYNCHRONIZE:
			this->active.synchronize();
			break;
		case ACTION_NOTIFY:
			break;
		case ACTION_DELETE:
			this->active.deleteFile(args[ARG_FILENAME]);
			break;
		case ACTION_DOWNLOAD:
			this->active.downloadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
		case ACTION_UPLOAD:
			this->active.uploadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
	}
	action.signal();
}

void Device::processAction(int actionType){
	std::map<std::string, std::string> args;

	/* Only passive channel used*/
	switch (actionType)
	{
		case ACTION_INITILIAZE:
			this->passive.synchronize();
			this->pushAction(Action(ACTION_SYNCHRONIZE, args));
			break;
		case ACTION_SYNCHRONIZE:
			this->passive.synchronize();
			break;
		case ACTION_NOTIFY:
			this->pushAction(Action(ACTION_SYNCHRONIZE, args));
			break;
		case ACTION_DELETE:
			this->passive.deleteFile();
			break;
		case ACTION_DOWNLOAD:
			this->passive.downloadFile();
			break;
		case ACTION_UPLOAD:
			this->passive.uploadFile();
			break;
	}
}

int Device::nextActionResquest(void)
{
	return this->passive.parseActionResquest();
}

void Device::pushAction(Action newAction)
{
	// lock
	this->actions.push(newAction);
}

Action Device::popAction(void)
{
	// lock
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
	//lock
	return this->actions.empty();
}
