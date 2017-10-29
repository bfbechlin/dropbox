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

void FileDiff::display(void) {

    std::vector<File>::iterator i;

    if(this->createdFiles.empty()) {
        std::cout << "\n\n[FileDiff]~: No new files.";
    } else {
        std::cout << "\n\n[FileDiff]~: New files found ";
        for(i = this->createdFiles.begin(); i != this->createdFiles.end(); i++) {

            std::cout<< "\n\t[+] " << (*i).getName();
        }
    }


    if(this->modifiedFiles.empty()) {
        std::cout << "\n\n[FileDiff]~: There are no recent modifications.";
    } else {
        std::cout << "\n\n[FileDiff]~: New modifications found in the file(s) above";
        for(i = this->modifiedFiles.begin(); i != this->modifiedFiles.end(); i++) {

            std::cout<< "\n\t[M] " << (*i).getName();
        }
    }

    if(this->deletedFiles.empty()) {
        std::cout << "\n\n[FileDiff]~: There are no recent deletions.";
    } else {
        std::cout << "\n\n[FileDiff]~: Recent deletions ";
        for(i = this->deletedFiles.begin(); i != this->deletedFiles.end(); i++) {

            std::cout<< "\n\t[-] " << (*i).getName();
        }
    }

}
