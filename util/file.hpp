#ifndef __FILE_HPP__
#define __FILE_HPP__

class File
{
	private:
		std::string name;
		std::string path;
		Timestamp modification;
		Timestamp access;
	public:
		File(std::path, std::name);
		bool isValid(void);
};

#endif
