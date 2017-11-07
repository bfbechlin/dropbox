#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <map>
#include <string>
#include <condition_variable>

#define ARG_FILENAME "fileName"
#define ARG_PATHNAME "pathName"

#define ACTION_INITILIAZE 	0
#define ACTION_SYNCHRONIZE	1
#define ACTION_NOTIFY 		2
#define ACTION_DELETE  		3
#define ACTION_DOWNLOAD 	4
#define ACTION_UPLOAD  		5

class Action{
	private:
		int type;
		std::map<std::string, std::string> arguments;
		std::condition_variable* signalVar;

	public:
		Action(void){};
		Action(int type);
		Action(int type, std::map<std::string, std::string> arguments);
		Action(int type, std::map<std::string, std::string> arguments,
			std::condition_variable* signalVar);

		int getType(void);
		void signal();
		std::string getTypeName(void);
		std::map<std::string, std::string> getArguments(void);
};

#endif
