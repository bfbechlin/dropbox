#include "foldermanager.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/inotify.h>

#define INOTIFY_EVENTS IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM | IN_MOVE_SELF
#define EVENT_SIZE  (sizeof (struct inotify_event))
#define BUF_LEN     4096

FolderManager::FolderManager(std::string path)
{
	if(path.back() == '/')
	{
		this->path = path.substr(0, path.size()-1);;
	}
	else
	{
		this->path = path;
	}

	if (File::exists(this->path) && !File::isDir(this->path))
	{
		fprintf(stderr, "[FolderManager]~: ERROR entry %s already exists and not is a diretory.\n",
			path.c_str());
			//exit(2);
	}
	if(!File::exists(this->path))
	{
		if(mkdir(this->path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0){
			fprintf(stderr, "[FolderManager]~: ERROR impossible to create folder %s.\n",
			this->path.c_str());
			//exit(2);
		}
	}

	this->inotifyFd = inotify_init1(IN_NONBLOCK);
	this->watcherFd = inotify_add_watch(this->inotifyFd, this->path.c_str(), INOTIFY_EVENTS);
	/* Clear all modification events */
	this->isModified();
}

FolderManager::~FolderManager()
{
	inotify_rm_watch(this->inotifyFd, this->watcherFd);
	close(this->inotifyFd);
}

std::string FolderManager::getPath(void)
{
	return std::string(this->path + '/');
}

bool FolderManager::isModified(void)
{
	char buf[BUF_LEN]
	   __attribute__ ((aligned(__alignof__(struct inotify_event))));
	const struct inotify_event *event;
	ssize_t len;
	char *ptr;
	bool modified = false;

	while((len = read(this->inotifyFd, buf, sizeof buf)) > 0)
	{
		for (ptr = buf; ptr < buf + len; ptr += EVENT_SIZE + event->len)
		{
			event = (const struct inotify_event*) ptr;
	      	if (event->len)
			{
			 	if(!File::isTemp(std::string(event->name)))
			  	{
	   				modified = true;
	   			}
			}
		}
	}
	return modified;
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
		if(File::isValid(this->path + '/' + fileName))
		{
			files.push_back(File(this->path, fileName));
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
		files.push_back(File(this->path, fileName));
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

std::string FolderManager::toString() const {

	std::string info(" ");

	info += "\nFolder FD:" + std::to_string(this->inotifyFd) + " at " + this->path + " being watched with descriptor WD:" + std::to_string(this->watcherFd) + ".\n";

	return info;
}
