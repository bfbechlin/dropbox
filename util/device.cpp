#include "device.hpp"

Device::Device(void)
{
	this->activeChannel = Communication();
	this->passiveChannel = Communication();
}

Device::Device(Communication activeChannel, Communication passiveChannel, FolderManager *folder)
{
	this->activeChannel = activeChannel;
	this->passiveChannel = passiveChannel;
	this->folder = folder;
}

void Device::initializeActive()
{

}
void Device::initializePassive()
{

}

void Device::synchronizeActive()
{

}

void Device::synchronizePassive()
{

}

void Device::notifyActive()
{

}

void Device::notifyPassive()
{

}

void Device::uploadActive()
{

}

void Device::uploadPassive()
{

}

void Device::downloadActive()
{

}

void Device::downloadPassive()
{

}
