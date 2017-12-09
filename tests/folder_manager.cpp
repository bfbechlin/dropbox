#include <iostream>
#include "foldermanager.hpp"
#include "action.hpp"

/*
* Test for get modification events at folderName
* usage ./folder_manager <folderName>
*/
int main(int argc, char* argv[])
{

	std::map<std::string, std::string> args;
	args[ARG_FILENAME] = "test.txt";
	args[ARG_PATHNAME] = "path";

	Action action1(ACTION_DOWNLOAD, args);
	std::cout << action1.encode() << '\n';
	Action action2(action1.encode());
	std::cout << action2.encode() << '\n';
	return 0;
}
