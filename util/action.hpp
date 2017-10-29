#ifndef __ACTION_HPP__
#define __ACTION_HPP__

enum actions{INITIALIZE, SYNCHRONIZE, NOTIFY, UPLOAD, DOWNLOAD};

class Action{
	private:
		actions type;
		std::string argument;
	public:
		Action(void);
		Action(actions type);
		Action(actions type, std::string argument);

		actions getType(void);
		std::string getArgument(void);
};

#endif
