#ifndef __FOLDERMANAGER_HPP__
#define __FOLDERMANAGER_HPP__

#include <map>
#include <string>
#include <vector>
#include "file.hpp"

typedef enum{
	DIFF_CREATED,
	DIFF_MODIFIED,
	DIFF_DELETED
}DIFF_ACTIONS;

class FolderManager
{
	private:
		std::string path;
	public:
		/*
			Initialize inotify
		*/
		FolderManager(std::string path);
		/*
		 	Verify if has ocurred a modification in this folder since the last
		 		call for this method or the constructor
		*/
		bool isModified(void);
		/*
			Get all valid files in this folder.
				e.g.:
				* it's not a temporary file -> ended in ~
				* it's not a folder descriptor- > entry like . and ..
		*/
		std::vector<File*> getFiles(void);
		/*
			Classificate received files in 3 groups comparing with this folder
			files. Like a git diff.
			DIFF_CREATED: files that are in 'files' and not in this folder
			DIFF_MODIFIED: files that are in both, but the modification Timestamp
				is bigger in 'files'
			DIFF_DELETED: files thar aren't in 'files' but are this folder.
		*/
		std::map<int, File*> diff(std::vector<File*> files);
};

#endif
