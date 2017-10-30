#ifndef __SERVERUSER_HPP__
#define __SERVERUSER_HPP__

class ServerUser
{
	private:
		std::string name;
		std::vector<Device*> devices;

	public:
		ServerUser(void){};
		ServerUser(std::string name, std::vector<Device*> devices);
		
		void processResquest(std::string actionType);
		void processAction(Action action);
		void notifyAll(void);
};
#endif
