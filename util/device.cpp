#include "device.hpp"

Device::Device(ActiveProcess active, PassiveProcess passive)
{
	this->active = active;
	this->passive = passive;
	this->endConn = false;
	this->buffer = "";
}

std::string Device::getMessage(void){
	std::string buff = this->buffer;
	this->buffer = "";
	return buff;
}

bool Device::isEndConnection(void){
	std::unique_lock<std::mutex> lck(this->endAcess);
	return this->endConn;
}

void Device::endConnection(void){
	std::unique_lock<std::mutex> lck(this->endAcess);
	this->endConn = true;
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
			this->active.merge();
			break;
		case ACTION_MERGE:
			this->active.merge();
			break;
		case ACTION_SYNCHRONIZE:
			this->active.synchronize();
			break;
		case ACTION_NOTIFY:
			break;
		case ACTION_DELETE:
			this->buffer = this->active.deleteFile(args[ARG_FILENAME]);
			break;
		case ACTION_DOWNLOAD:
			this->buffer = this->active.downloadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
		case ACTION_UPLOAD:
			this->active.uploadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
		case ACTION_LIST:
			this->buffer = this->active.list();
			break;
		case ACTION_EXIT:
			this->endConnection();
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
			this->passive.merge();
			this->pushAction(Action(ACTION_MERGE, args));
			break;
		case ACTION_MERGE:
			this->passive.merge();
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
		case ACTION_LIST:
			this->passive.list();
			break;
		case ACTION_EXIT:
			this->endConnection();
			break;
	}
}

int Device::nextActionResquest(void)
{
	return this->passive.parseActionResquest();
}

void Device::pushAction(Action newAction)
{
	std::unique_lock<std::mutex> lck(this->queueAcess);
	this->actions.push(newAction);
}

Action Device::popAction(void)
{
	std::unique_lock<std::mutex> lck(this->queueAcess);
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
	std::unique_lock<std::mutex> lck(this->queueAcess);
	return this->actions.empty();
}
