#include "database.hpp"
#include <sys/stat.h>

/* Init Singleton Instance */
Database* database = 0;

Database::Database(void)
{
	this->path = std::string("");
}

Database::Database(std::string path)
{
	struct stat sb;

	this->path = path;

	if(stat(path.c_str(), &sb) == 0 && !S_ISDIR(sb.st_mode))
	{
		fprintf(stderr, "[Database]~: ERROR entry %s already exists and not is a diretory.\n",
			path.c_str());
			exit(3);
	}
	if(stat(path.c_str(), &sb) < 0)
	{
		if(mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0){
			fprintf(stderr, "[Database]~: ERROR impossible to create folder %s.\n",
			path.c_str());
			exit(3);
		}
	}
}

std::string Database::getPath()
{
	return this->path;
}

bool Database::setPath(std::string newPath)
{
	struct stat sb;

	if ((stat(newPath.c_str(), &sb) == 0 && !S_ISDIR(sb.st_mode)) ||
		(stat(newPath.c_str(), &sb) < 0))
	{
		return false;
	}
	this->path = newPath;
	return true;
}
