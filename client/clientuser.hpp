#ifndef __CLIENTUSER_HPP__
#define __CLIENTUSER_HPP__

class ClientUser
{
	private:
		std::string name;
		Device* device;
		
	public:
		ClientUser(void){};
		ClientUser(std::string name, Device* device){};
		void processResquest(std::string actionType);
		void processAction(Action action);
};

#endif
