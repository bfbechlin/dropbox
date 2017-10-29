#ifndef __ACTION_HPP__
#define __ACTION_HPP__

#include <map>
#include <string>

enum actions{INITIALIZE, SYNCHRONIZE, NOTIFY, UPLOAD, DOWNLOAD};
#define ARG_FILENAME  "fileName"
#define ARG_DESTPATH "destinationPath"
#define ARG_SOURCEPATH "sourcePath"

class Action{
	private:
		actions type;
		std::map<std::string, std::string> arguments;
	public:
		Action(void);
		Action(actions type);
		Action(actions type, std::map<std::string, std::string> arguments);

		actions getType(void);
		std::map<std::string, std::string> getArguments(void);
};

#endif
