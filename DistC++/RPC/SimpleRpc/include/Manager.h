/*
 * Copyright 1990, 1991, 1992-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: Manager.h,v 1.3 1998/11/20 08:28:46 nmcl Exp $
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

    Boolean packInto (Buffer&);
    Boolean unpackFrom (Buffer&);
    
    pid_t pid;
    int useCount;
    PortAddress address;
    char *service;
    Uid serverID;
    ClientTable *clients;
#ifdef WIN32
    ProtocolType _protocolName;
#endif
    
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

    Boolean processInformation (Buffer&);
    
private:
    Boolean isAlive (const ProcessTable& serverId) const;
    void  terminateServer (const ProcessTable& serverId) const;
    char* locateService (const char*) const;

#ifdef WIN32
    void  tcpTerminate (const ProcessTable& serverId) const;
    void  udpTerminate (const ProcessTable& serverId) const;
#endif
    
    Port* localPort;
    ProcessTable* head;
    char* searchPath[MAX_SEARCHPATH+1];    
};

#endif
