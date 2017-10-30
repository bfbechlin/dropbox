#include "action.hpp"

Action::Action(std::string type)
{
	this->type = type;
}

Action::Action(std::string type, std::map<std::string, std::string> arguments)
{
	this->type = type;
	this->arguments = arguments;
}

std::string Action::getType(void)
{
	return this->type;
}
std::map<std::string, std::string> Action::getArguments(void)
{
	return this->arguments;
}
