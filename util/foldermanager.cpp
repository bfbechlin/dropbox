#include "foldermanager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

FolderManager::FolderManager(std::string path)
{
	this->path = path;

	if (File::isDir(this->path));
	{
		fprintf(stderr, "[FolderManager]~: ERROR entry %s already exists and not is a diretory.\n",
			path.c_str());
			exit(2);
	}
	if(!File::exists(this->path))
	{
		if(mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0){
			fprintf(stderr, "[FolderManager]~: ERROR impossible to create folder %s.\n",
			this->path.c_str());
			exit(2);
		}
	}

}

File FolderManager::makeFile(std::string path, std::string name)
{
	struct stat buffer;
	std::string fullPath = path + name;
	stat(fullPath.c_str(),&buffer);
	File file(name, buffer.st_atim, buffer.st_mtim);
	return file;
}

std::vector<File> FolderManager::getFiles(void)
{
    DIR *dir;
    std::string fullPath;
    struct dirent *file;
    std::vector<File> files;

    if(!(dir = opendir(this->path.c_str())))
	{
        fprintf(stderr, "[FolderManager]~: ERROR on opening dir %s\nExiting...\n", this->path.c_str());
        exit(1);
    }

    while ((file = readdir(dir)) != NULL)
	{
		std::string fileName(file->d_name);
		fullPath = this->path;
        if(fullPath.back() != '/')
		{
			fullPath += "/";
		}

		if(File::isValid(fullPath + fileName))
		{
			files.push_back(this->makeFile(fullPath, std::string(file->d_name)));
		}
    }
    closedir(dir);

    return files;
}

std::vector<File> FolderManager::getAllFiles(void)
{
	DIR *dir;
    std::string fullPath;
    struct dirent *file;
    std::vector<File> files;

    if(!(dir = opendir(this->path.c_str())))
	{
        fprintf(stderr, "[FolderManager]~: ERROR on opening dir %s\nExiting...\n", this->path.c_str());
        exit(1);
    }

    while ((file = readdir(dir)) != NULL)
	{
		std::string fileName(file->d_name);
		fullPath = this->path;
        if(fullPath.back() != '/')
		{
			fullPath += "/";
		}

		files.push_back(this->makeFile(fullPath, std::string(file->d_name)));
    }
    closedir(dir);

    return files;
}

FileDiff FolderManager::diff(std::vector<File> remoteFiles)
{
    std::vector<File>::iterator i;
    std::vector<File>::iterator j;
	std::vector<File> localFiles = this->getFiles();
	std::vector<File> created, modified, deleted;

	bool found;

	/* Created */
	for (i = remoteFiles.begin(); i != remoteFiles.end(); i++)
	{
		found = false;
        for(j = localFiles.begin(); j != localFiles.end(); j++)
		{
            if((*j).getName() == (*i).getName())
			{
				found = true;
                break;
            }
        }
		if(!found)
		{
			created.push_back((*i));
		}
    }

	/* Deleted */
	for (i = localFiles.begin(); i != localFiles.end(); i++)
	{
		found = false;
        for(j = remoteFiles.begin(); j != remoteFiles.end(); j++)
		{
            if((*j).getName() == (*i).getName())
			{
				found = true;
                break;
            }
        }
		if(!found)
		{
			deleted.push_back((*i));
		}
    }

	/* Modified */
    for (i = localFiles.begin(); i != localFiles.end(); i++)
	{
        for(j = remoteFiles.begin(); j != remoteFiles.end(); j++)
		{
            if((*j).getName() == (*i).getName())
			{
                if((*i).getModification() < (*j).getModification())
				{
                    modified.push_back(*i);
					break;
                }
            }
        }
    }

    return FileDiff(created, modified, deleted);
}
