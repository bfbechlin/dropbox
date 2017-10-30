#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <map>
#include <string>

#define ARG_FILENAME "fileName"
#define ARG_PATHNAME "pathName"

#define ACTION_INITILIAZE 	"init"
#define ACTION_SYNCHROZIZE	"synchronize"
#define ACTION_NOTIFY 		"notify"
#define ACTION_DELETE  		"delete"
#define ACTION_DOWNLOAD 	"download"
#define ACTION_UPLOAD  		"upload"

class Action{
	private:
		std::string type;
		std::map<std::string, std::string> arguments;
	public:
		Action(void){};
		Action(std::string type);
		Action(std::string type, std::map<std::string, std::string> arguments);

		std::string getType(void);
		std::map<std::string, std::string> getArguments(void);
};

#endif
