#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include <string>

class Database
{
	private:
		std::string path;

	public:
		Database();
		Database(std::string path);

		std::string getPath();
		bool setPath(std::string);
};

/* Database Singleton*/
extern Database* database;

#endif
