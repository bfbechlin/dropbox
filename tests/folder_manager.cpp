#include <iostream>
#include "foldermanager.hpp"

/*
* Test for get modification events at folderName
* usage ./folder_manager <folderName>
*/
int main(int argc, char* argv[])
{

	std::string path(argv[1]);
	std::string buffer;

	FolderManager folder(path);

	std::cout << "Verifing for modification at " << path << "\n";
	std::cout << (folder.isModified() ? "True" : "False") << "\n";
	std::cout << "write anything and enter to continue:\n";
	std::cin >> buffer;

	std::cout << "Verifing for modification at " << path << "\n";
	std::cout << (folder.isModified() ? "True" : "False") << "\n";
	std::cout << "write anything and enter to continue:\n";
	std::cin >> buffer;

	std::cout << "Verifing for modification at " << path << "\n";
	std::cout << (folder.isModified() ? "True" : "False") << "\n";
	std::cout << "write anything and enter to continue:\n";
	std::cin >> buffer;
	return 0;
}
