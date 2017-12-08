#ifndef __FOLDERDIFF_H__
#define __FOLDERDIFF_H__

#include "file.hpp"
#include <vector>

class FolderDiff
{

	private:
	    std::vector<File> createdFiles;
	    std::vector<File> modifiedFiles;
			std::vector<File> deletedFiles;

	public:
	    FolderDiff(void){};
			FolderDiff(std::vector<File> createdFiles, std::vector<File> modifiedFiles,
			std::vector<File> deletedFiles);

			void createdFile(File file);
	    void modifiedFile(File file);
	    void deletedFile(File file);

	    std::vector<File> getCreatedFiles(void);
	    std::vector<File> getModifiedFiles(void);
			std::vector<File> getDeletedFiles(void);

			std::string toString();

};


#endif
