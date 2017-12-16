#ifndef __BACKUP_HPP__
#define __BACKUP_HPP__

#include "database.hpp"
#include "foldermanager.hpp"
#include "servercomm.hpp"
#include "loggedusers.hpp"
#include "device.hpp"
#include "backupservers.hpp"
#include "serverinfo.hpp"

#include "../client/clientcomm.hpp"
#include "../client/clientuser.hpp"
#include <iostream>
#include <thread>

void backupSetup(ServerInfo primaryServer, BackupServers* servers);

#endif
