/*
 * Copyright 1990, 1991, 1992-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

#ifndef MANAGER_DEF_H_
#define MANAGER_DEF_H_

/*
 *  $Id: Manager_def.h,v 1.3 1998/11/20 08:28:39 nmcl Exp $
 */

#define MANAGER_PORT_GLOBAL 5001
#define MANAGER_PORT_LOCAL  5002

enum managerOpcodes { DO_INITIATE = 1, DO_TERMINATE = 2, MANAGER_DIE = 3, BIND_INFO = 4, MODE_CHANGE };

enum ManagerModes { MANAGER_SINGLEUSER, MANAGER_MULTIUSER };

#define DGRAM_PROTOCOL "DGRAM"
#define STREAM_PROTOCOL "STREAM"
#define NO_MANAGER "NO_MANAGER"

#define MANAGER_WAIT_TIME 5000 /* Wait time for server to reply to manager. */

#define PATH_DELIM ";"
#define MAX_SEARCHPATH 10

#endif
