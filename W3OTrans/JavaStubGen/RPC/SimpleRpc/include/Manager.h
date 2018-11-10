/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: Manager.h,v 1.3 1996/04/03 13:01:16 nmcl Exp $
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTCONTROL_H_
#  include <RPC/SimpleRpc/ClientControl.h>
#endif

class ClientTable
{
public:    
    ClientTable (const Uid&, long);
    ~ClientTable ();

    Boolean isPresent (const Uid&, long) const;
    Boolean isCurrent (const Uid&, long) const;
    void    add       (const Uid&, long);
    void    remove    (const Uid&, long);
    
    ClientTable* next;

private:
    Uid clientID;
    long sequenceNumber;
};

class ProcessTable
{
public:
    enum Opcode { OLD_CLIENT, NEW_CLIENT, NOT_FOUND };
    
    ProcessTable ();
    ~ProcessTable ();

    Opcode isPresent (const char*, const Uid&, const Uid&, long);
    
    pid_t pid;
    int useCount;
    PortAddress address;
    char *service;
    Uid serverID;
    ClientTable *clients;
    
    ProcessTable *next;
};

class Manager
{
public:
    Manager (char** path);
    ~Manager ();

    Boolean Initiate (const char*, PortAddress&, const Uid&,
		      const Uid&, long, ProtocolType);
    void Terminate (const char*, const Uid&, const Uid&, long);

private:
    char* locateService (const char*);

    Port* localPort;
    ProcessTable* head;
    char** searchPath;
};

#endif
