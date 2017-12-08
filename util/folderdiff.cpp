#include <iostream>
#include "folderdiff.hpp"

FolderDiff::FolderDiff(std::vector<File> createdFiles, std::vector<File> modifiedFiles,
	std::vector<File> deletedFiles)
{
	this->createdFiles = createdFiles;
	this->modifiedFiles = modifiedFiles;
	this->deletedFiles = deletedFiles;
}
void FolderDiff::createdFile(File file)
{
    this->createdFiles.push_back(file);
}

void FolderDiff::modifiedFile(File file)
{
    this->modifiedFiles.push_back(file);
}

void FolderDiff::deletedFile(File file)
{
    this->deletedFiles.push_back(file);
}

std::vector<File> FolderDiff::getCreatedFiles(void)
{
    return this->createdFiles;
}

std::vector<File> FolderDiff::getDeletedFiles(void)
{
    return this->deletedFiles;
}

std::vector<File> FolderDiff::getModifiedFiles(void) {

    return this->modifiedFiles;
}


std::string FolderDiff::toString() {

    std::vector<File>::iterator i;
		std::string info("[FolderDiff]~:");

	if(this->createdFiles.empty() && this->modifiedFiles.empty() && this->deletedFiles.empty()){
		info += " no differences found.";
	} else{
		info += " differences found:";
	}
    for(i = this->createdFiles.begin(); i != this->createdFiles.end(); i++) {

        info += "\n\t[+] " + (*i).getName();
    }

    for(i = this->modifiedFiles.begin(); i != this->modifiedFiles.end(); i++) {

        info += "\n\t[M] " + (*i).getName();
    }

    for(i = this->deletedFiles.begin(); i != this->deletedFiles.end(); i++) {

        info += "\n\t[-] " + (*i).getName();
    }

	return info;
}
