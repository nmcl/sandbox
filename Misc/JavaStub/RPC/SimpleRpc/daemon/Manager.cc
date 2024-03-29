/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Manager.cc,v 1.6 1996/04/03 13:00:38 nmcl Exp $
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SYS_WAIT_H_
#  include <System/sys/wait.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

#ifndef MANAGER_DEF_H_
#  include <RPC/SimpleRpc/Manager_def.h>
#endif

#ifndef MANAGER_H_
#  include "Manager.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif


ClientTable::ClientTable ( const Uid& id, long seqNo )
			 : next(0),
			   clientID(id),
			   sequenceNumber(seqNo)
{
}

ClientTable::~ClientTable ()
{
}

Boolean ClientTable::isCurrent ( const Uid& id, long seqNo ) const
{
    return (Boolean) ((clientID == id) && (sequenceNumber == seqNo));
}

Boolean ClientTable::isPresent ( const Uid& id, long seqNo ) const
{
    Boolean found = FALSE;
    const ClientTable* tmp = this;
    
    while ((!found) && (tmp))
    {
	if (tmp->isCurrent(id, seqNo))
	    found = TRUE;
	else
	    tmp = tmp->next;
    }
    
    return found;
}

void ClientTable::add ( const Uid& id, long seqNo )
{
    ClientTable* tmp = this;
    
    while (tmp->next)
	tmp = tmp->next;
    
    tmp->next = new ClientTable(id, seqNo);
}

void ClientTable::remove ( const Uid& id, long seqNo )
{
    ClientTable *tmp = this, *trail = (ClientTable*) 0;
    
    while (tmp)
    {
	if (tmp->isCurrent(id, seqNo))
	{
	    trail->next = tmp->next;
	    delete tmp;
	    return;
	}
	else
	{
	    trail = tmp;
	    tmp = tmp->next;
	}
    }
}


ProcessTable::ProcessTable ()
			   : pid(-1),
			     useCount(1),
			     service(0),
			     clients(0),
			     next(0)
{
}

ProcessTable::~ProcessTable ()
{
    if (service)
	::delete [] service;
}

ProcessTable::Opcode ProcessTable::isPresent ( const char* serv, const Uid& servID,
					       const Uid& clientID, long seqNo )
{
    Boolean found = ((::strcmp(service, serv) == 0) && (serverID == servID));

    if ((found) && (clients))
    {
	found = clients->isPresent(clientID, seqNo);
	if (!found)
	{
	    clients->add(clientID, seqNo);
	    return NEW_CLIENT;
	}
	else
	    return OLD_CLIENT;
    }
    
    return NOT_FOUND;
}


Manager::Manager (char** path)
		 : head(0),
		   searchPath(path)
{
    PortAddress localPortAddr(MANAGER_PORT_LOCAL);
    localPort = new Port(localPortAddr);
}

Manager::~Manager ()
{
    ProcessTable* tmp = head;
    
    while (tmp)
    {
	tmp = head->next;
	delete head;
	head = tmp;
    }
    
    if (localPort != NULL)
	delete localPort;
}

char* Manager::locateService ( const char* service )
{
    int index = 0;
    char* fullServiceName = (char*) 0;
    struct stat buf;
    
    while (searchPath[index])
    {
	::memset(fullServiceName, '\0', ::strlen(service)+::strlen(searchPath[index])+2);
	::memcpy(fullServiceName, SERVER_LOCATION_Q, ::strlen(searchPath[index]));
	::memcpy(fullServiceName+::strlen(searchPath[index]), "/", 1);
	::memcpy(fullServiceName+::strlen(searchPath[index])+1, service, ::strlen(service));

	if (::stat(fullServiceName, &buf) == 0)
	    return fullServiceName;
    
	::delete [] fullServiceName;
    }
    
    return 0;
}

Boolean Manager::Initiate ( const char* service, PortAddress& serverAddr,
			    const Uid& serverID,
			    const Uid& clientID, long seqNo,
			    ProtocolType protocol)
{
    ProcessTable::Opcode found = ProcessTable::NOT_FOUND;
    ProcessTable* tmp = head;
    char* fullServiceName = 0;
    char* protocolName    = ((protocol == DGRAM) ? "DGRAM" : "STREAM");

    while ((found == ProcessTable::NOT_FOUND) && (tmp))
    {
	found = tmp->isPresent(service, serverID, clientID, seqNo);
        if (found == ProcessTable::NOT_FOUND)
	    tmp = tmp->next;
    }

    if (found == ProcessTable::NOT_FOUND)
    {    
	pid_t p_id;
	
	/* Check that server is available */

	fullServiceName = locateService(service);
	
	if (fullServiceName == 0)
	    return FALSE;

	p_id = ::fork();
		
	if (p_id == 0)
	{
	    pid_t p_id;
		    
	    p_id = ::fork();
		    
	    if (p_id == 0)
	    {
#ifdef DEBUG
		cout << "**Manager about to exec " << fullServiceName << endl;
#endif		
		if (::execl(fullServiceName, service, protocolName, 0) == -1)
		{
		    ::perror("MANAGER: Initiate, exec ");
		    return FALSE;
		}
	    }
	    else if (p_id < 0)
		::perror("MANAGER: Initiate, sub-fork ");
	    else
		exit(0);

	    exit(1);
	}
	else if (p_id < 0)
	{
	    ::perror("MANAGER: Initiate, fork ");
	    exit(1);
	}
	else
	{
#ifdef HAVE_UNION_WAIT
	    union wait statusp;
#else
	    int   statusp;
#endif
	    pid_t wait_res;
		    
	    wait_res = ::wait(&statusp);

	    if ((wait_res <= 0) || (! WIFEXITED(statusp)))
		::perror("MANAGER: Initiate, wait ");
	}
	      
	PortAddress addr;
	Buffer      buff;
	int number = -1;
	Buffer* RecBuff[1];

	RecBuff[0] = &buff;

#ifdef DEBUG
	cout << "**Manager - waiting to receive bind request from server" << endl;
#endif
	
	if (localPort->receiveMessage(addr, RecBuff, number) == Port::DONE)
	{
	    tmp = new ProcessTable;
	    tmp->next = head;
	    head = tmp;

	    head->address.unpack(buff);
	    head->serverID = serverID;
	    buff.unpack(head->pid);
	    head->service = ::new char[::strlen(service)+1];
	    ::memset(head->service, '\0', ::strlen(service)+1);
	    ::memcpy(head->service, service, ::strlen(service));
	    serverAddr = head->address;
	    head->clients = new ClientTable(clientID, seqNo);

#ifdef DEBUG
	    cout << "**Manager - received bind information : " << serverAddr << " and " << serverID << endl;
#endif	    
	}
        else
	    ::perror("MANAGER: Initiate, receive message ");
    }
    else
    {
	serverAddr = tmp->address;
	if (found == ProcessTable::NEW_CLIENT)
	    tmp->useCount++;
    }

    return TRUE;
}

void Manager::Terminate ( const char* service, const Uid& serverID,
			  const Uid& clientID, long seqNo )
{
    ProcessTable::Opcode found = ProcessTable::NOT_FOUND;
    ProcessTable *tmp = head, *trail = 0;

#ifdef DEBUG
    cout << "**Manager - request to terminate " << service << " and " << serverID << endl;
#endif
    
    while ((found == ProcessTable::NOT_FOUND) && (tmp))
    {
	found = tmp->isPresent(service, serverID, clientID, seqNo);
        if (found == ProcessTable::NOT_FOUND)
	{
	    trail = tmp;
	    tmp = tmp->next;
	}
    }

    if (found != ProcessTable::NOT_FOUND)
    {
	if (tmp->clients->isCurrent(clientID, seqNo))
	{
	    ClientTable* t_ct = tmp->clients;
	    tmp->clients = tmp->clients->next;
	    delete t_ct;
	}
	else
	    tmp->clients->remove(clientID, seqNo);
	
	tmp->useCount--;

	if (tmp->useCount == 0)
	{
	    ::kill(tmp->pid, SIGTERM);
	    if (trail == 0)
		head = tmp->next;
	    else
		trail->next = tmp->next;

	    delete tmp;
	}
    }
    else
        cerr << "MANAGER: Terminate, server not found" << endl;
}

