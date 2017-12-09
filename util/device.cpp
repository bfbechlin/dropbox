#include "device.hpp"

Device::Device(ActiveProcess active, PassiveProcess passive)
{
	this->active = active;
	this->passive = passive;
	this->endConn = false;
}

std::string Device::getMessage(void)
{
	return this->active.getInfo();
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
	switch (actionType)
	{
		case ACTION_MERGE:
			this->active.sendActionResquest(action);
			this->active.merge(&(this->actions));
			break;
		case ACTION_SYNCHRONIZE:
			this->active.sendActionResquest(action);
			this->active.synchronize(&(this->actions));
			break;
		case ACTION_NOTIFY:
			this->active.sendActionResquest(action);
			break;
		case ACTION_DELETE:
			this->active.sendActionResquest(action);
			this->active.deleteFile(args[ARG_FILENAME]);
			break;
		case ACTION_SELF_DELETE:
			this->active.selfDeleteFile(args[ARG_FILENAME]);
			break;
		case ACTION_DOWNLOAD:
			this->active.sendActionResquest(action);
			this->active.downloadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
		case ACTION_UPLOAD:
			this->active.sendActionResquest(action);
			this->active.uploadFile(args[ARG_PATHNAME], args[ARG_FILENAME]);
			break;
		case ACTION_LIST:
			this->active.sendActionResquest(action);
			this->active.list();
			break;
		case ACTION_EXIT:
			this->active.sendActionResquest(action);
			this->endConnection();
			this->active.exit();
			break;
	}
	action.signal();
}

void Device::processAction(Action action){

	int actionType = action.getType();

	/* Only passive channel used*/
	switch (actionType)
	{
		case ACTION_MERGE:
			this->passive.merge();
			break;
		case ACTION_SYNCHRONIZE:
			this->passive.synchronize();
			break;
		case ACTION_NOTIFY:
			this->actions.pushBack(Action(ACTION_SYNCHRONIZE));
			break;
		case ACTION_DELETE:
			this->passive.deleteFile(&(this->actions));
			break;
		case ACTION_DOWNLOAD:
			this->passive.downloadFile();
			break;
		case ACTION_UPLOAD:
			this->passive.uploadFile(&(this->actions));
			break;
		case ACTION_LIST:
			this->passive.list();
			break;
		case ACTION_EXIT:
			this->endConnection();
			this->passive.exit();
			break;
	}
}

Action Device::nextActionResquest(void)
{
	return this->passive.parseActionResquest();
}
