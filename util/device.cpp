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
