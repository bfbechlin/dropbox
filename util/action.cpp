#include "action.hpp"

Action::Action(int type)
{
	std::map<std::string, std::string> args;
	this->type = type;
	this->arguments = args;
	this->signalVar = NULL;
}

Action::Action(std::string encoded)
{
	int args;
	std::size_t pos;
	std::string key, value, line;
	try {
		pos = encoded.find(" ");
		this->type = std::stoi(encoded.substr(0, pos));
		encoded = encoded.substr(pos+1, std::string::npos);
		pos = encoded.find(" ");
		args = std::stoi(encoded.substr(0, pos));
		encoded = encoded.substr(pos+1, std::string::npos);
		for (int i = 0; i < args; i++) {
			pos = encoded.find(";");
			line = encoded.substr(0, pos);
			encoded = encoded.substr(pos+1, std::string::npos);
			pos = line.find("=");
			key = line.substr(0, pos);
			value = line.substr(pos+1, std::string::npos);
			this->arguments[key] = value;
		}
 	} catch (...) {
		this->type = -1;
	}
	this->signalVar = NULL;
}

Action::Action(int type, std::map<std::string, std::string> arguments)
{
	this->type = type;
	this->arguments = arguments;
	this->signalVar = NULL;
}

Action::Action(int type, std::condition_variable* signalVar)
{
	this->type = type;
	this->signalVar = signalVar;
}

Action::Action(int type, std::map<std::string, std::string> arguments,
	std::condition_variable* signalVar)
{
	this->type = type;
	this->arguments = arguments;
	this->signalVar = signalVar;
}

void Action::signal(void){
	if(this->signalVar != NULL)
	{
		this->signalVar->notify_all();
	}
}

int Action::getType(void)
{
	return this->type;
}

std::string Action::encode(void){
	std::string encoded(std::to_string(this->type) + " " + std::to_string(this->arguments.size()) + " ");
	for (std::map<std::string,std::string>::iterator it=this->arguments.begin(); it!=this->arguments.end(); ++it)
   		encoded += std::string(it->first) + "=" + std::string(it->second) + ";";
	return encoded;
}

std::string Action::getTypeName(void)
{
	switch (this->type) {
		case ACTION_INVALID:
			return std::string("Invalid");
		case ACTION_MERGE:
			return std::string("Merge");
		case ACTION_SYNCHRONIZE:
			return std::string("Synchronize");
		case ACTION_NOTIFY:
			return std::string("Notify");
		case ACTION_NOTIFY_ALL:
			return std::string("NotifyAll");
		case ACTION_NOTIFY_OTHERS:
			return std::string("NotifyOthers");
		case ACTION_DELETE:
			return std::string("Delete");
		case ACTION_SELF_DELETE:
			return std::string("SelfDelete");
		case ACTION_DOWNLOAD:
			return std::string("Download");
		case ACTION_UPLOAD:
			return std::string("Upload");
		case ACTION_LIST:
			return std::string("List");
		case ACTION_EXIT:
			return std::string("Exit");
		case ACTION_BACKUPSERVERS:
			return std::string("UpdateBackupList");
		default:
			return std::string("");
	}
}

std::map<std::string, std::string> Action::getArguments(void)
{
	return this->arguments;
}
