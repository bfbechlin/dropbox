#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "foldermanager.hpp"

FolderManager::FolderManager(std::string path)
{
	struct stat sb;

	this->path = path;

	if (stat(path.c_str(), &sb) == 0 && !S_ISDIR(sb.st_mode))
	{
		fprintf(stderr, "[FolderManager]~: ERROR entry %s already exists and not is a diretory.\n",
			path.c_str());
			exit(2);
	}
	if(stat(path.c_str(), &sb) < 0)
	{
		if(mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0){
			fprintf(stderr, "[FolderManager]~: ERROR impossible to create folder %s.\n",
			path.c_str());
			exit(2);
		}
	}

}
