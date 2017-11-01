#include <iostream>
#include "filediff.hpp"

FileDiff::FileDiff(std::vector<File> createdFiles, std::vector<File> modifiedFiles,
	std::vector<File> deletedFiles)
{
	this->createdFiles = createdFiles;
	this->modifiedFiles = modifiedFiles;
	this->deletedFiles = deletedFiles;
}
void FileDiff::createdFile(File file)
{
    this->createdFiles.push_back(file);
}

void FileDiff::modifiedFile(File file)
{
    this->modifiedFiles.push_back(file);
}

void FileDiff::deletedFile(File file)
{
    this->deletedFiles.push_back(file);
}

std::vector<File> FileDiff::getCreatedFiles(void)
{
    return this->createdFiles;
}

std::vector<File> FileDiff::getDeletedFiles(void)
{
    return this->deletedFiles;
}

std::vector<File> FileDiff::getModifiedFiles(void) {

    return this->modifiedFiles;
}


std::string FileDiff::toString() {

    std::vector<File>::iterator i;
		std::string info(" ");

    if(this->createdFiles.empty()) {
        info += "\n\n[FileDiff]~: No new files.";
    } else {
        info += "\n\n[FileDiff]~: New files found ";
        for(i = this->createdFiles.begin(); i != this->createdFiles.end(); i++) {

            info += "\n\t[+] " + (*i).getName();
        }
    }


    if(this->modifiedFiles.empty()) {
        info += "\n\n[FileDiff]~: There are no recent modifications.";
    } else {
        info += "\n\n[FileDiff]~: New modifications found in the file(s) above";
        for(i = this->modifiedFiles.begin(); i != this->modifiedFiles.end(); i++) {

            info += "\n\t[M] " + (*i).getName();
        }
    }

    if(this->deletedFiles.empty()) {
        info += "\n\n[FileDiff]~: There are no recent deletions.";
    } else {
        info += "\n\n[FileDiff]~: Recent deletions ";
        for(i = this->deletedFiles.begin(); i != this->deletedFiles.end(); i++) {

            info += "\n\t[-] " + (*i).getName();
        }
    }

		return info;
}
