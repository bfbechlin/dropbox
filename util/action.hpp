#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <map>
#include <string>
#include <condition_variable>

#define ARG_FILENAME "fileName"
#define ARG_PATHNAME "pathName"

#define ACTION_INITILIAZE 		0
#define ACTION_MERGE  			1
#define ACTION_SYNCHRONIZE		2
#define ACTION_NOTIFY 			3
#define ACTION_NOTIFY_OTHERS 	4
#define ACTION_NOTIFY_ALL 		5
#define ACTION_DELETE  			6
#define ACTION_SELF_DELETE 		7
#define ACTION_DOWNLOAD 		8
#define ACTION_UPLOAD  			9
#define ACTION_LIST  			10
#define ACTION_EXIT  			11

class Action{
	private:
		int type;
		std::map<std::string, std::string> arguments;
		std::condition_variable* signalVar;

	public:
		Action(void){};
		Action(int type);
		Action(int type, std::map<std::string, std::string> arguments);
		Action(int type, std::condition_variable* signalVar);
		Action(int type, std::map<std::string, std::string> arguments,
			std::condition_variable* signalVar);

		int getType(void);
		void signal();
		std::string getTypeName(void);
		std::map<std::string, std::string> getArguments(void);
};

#endif
