#ifndef __FILE_HPP__
#define __FILE_HPP__

#include "timestamp.hpp"
#include <string>

class File
{
	private:
		std::string name;
		Timestamp modification;
		Timestamp access;
	public:
		File(void);
		File(std::string name);
		File(std::string name, Timestamp access, Timestamp mod);

		std::string getName(void);
		Timestamp getModification(void);
		Timestamp getAccess(void);

		std::string toString(void) const;
		bool isValid(void);
};

#endif
