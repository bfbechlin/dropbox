#include "file.hpp"

File::File(){
	this->name = std::string("");
	this->access = Timestamp();
	this->modification = Timestamp();
}

File::File(std::string name){
	this->name = name;
	this->access = Timestamp();
	this->modification = Timestamp();
}

File::File(std::string name, Timestamp access, Timestamp mod)
{
	this->name = name;
	this->access = access;
	this->modification = mod;
}


std::string File::getName(void)
{
	return this->name;
}

Timestamp File::getAccess(void)
{
	return this->access;
}

Timestamp File::getModification(void)
{
	return this->modification;
}

std::string File::toString() const{
	return std::string("  File: " + this->name +
		"\nAccess: " + this->access.toDate() +
		"\nModify: " + this->modification.toDate());
}

bool File::isValid(void)
{
	// IMPLEMENT
	return true;
}
