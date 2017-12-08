#ifndef __FILEACCESSCONTROL_HPP__
#define __FILEACCESSCONTROL_HPP__

#define FILEACCESS_READ 1
#define FILEACCESS_WRITE 2

#include <mutex>
#include <string>
#include <vector>

class FileAccessControl
{
	private:
		std::mutex rw;
		std::mutex mutexRead;
		std::mutex mutexWrite;

		int readers;
		int writers;
		std::string name;

	public:
		FileAccessControl(std::string fileName);

		void block(int type);
		void release(int type);
		bool noLock(void);
};

#endif
