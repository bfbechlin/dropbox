#ifndef __FILEDIFF_H__
#define __FILEDIFF_H__

#include "file.hpp"
#include <vector>

class FileDiff
{

	private:
	    std::vector<File> createdFiles;
	    std::vector<File> modifiedFiles;
		std::vector<File> deletedFiles;

	public:
	    FileDiff(void){};
		FileDiff(std::vector<File> createdFiles, std::vector<File> modifiedFiles,
			std::vector<File> deletedFiles);

		void createdFile(File file);
	    void modifiedFile(File file);
	    void deletedFile(File file);

	    std::vector<File> getCreatedFiles(void);
	    std::vector<File> getModifiedFiles(void);
		std::vector<File> getDeletedFiles(void);
	    void display(void);
};


#endif
