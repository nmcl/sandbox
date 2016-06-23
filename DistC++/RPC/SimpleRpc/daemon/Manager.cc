/*
 * Copyright (C) 1990-1997, 1998,
 * Department of Computing Science,
 * University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Manager.cc,v 1.5 1998/11/20 08:28:44 nmcl Exp $
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
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

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef WIN32

#ifndef SYS_WAIT_H_
#  include <System/sys/wait.h>
#endif

#else

#include <System/stdio.h>
#include <System/wchar.h>
#include <System/process.h>

#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
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
    ClientTable *tmp = this;
    ClientTable *trail = (ClientTable*) 0;
    
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
#ifdef WIN32
			     _protocolName(DGRAM),
#endif
			     next(0)
{
}

ProcessTable::~ProcessTable ()
{
    if (clients)
    {
	ClientTable* ptr = clients;

	while (clients)
	{
	    clients = clients->next;
	    delete ptr;
	    ptr = clients;
	}
    }
    
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

Boolean ProcessTable::packInto (Buffer& buff)
{
    if (buff.pack(pid) && buff.pack(useCount))
    {
	if (buff.packNonMappedString(service) && serverID.pack(buff))
	    return TRUE;
    }

    return FALSE;
}

Boolean ProcessTable::unpackFrom (Buffer& buff)
{
    if (buff.unpack(pid) && buff.unpack(useCount))
    {
	if (service)
	{
	    ::delete [] service;
	    service = 0;
	}

	if (buff.unpack(service) && serverID.unpack(buff))
	    return TRUE;
    }

    return FALSE;
}

Manager::Manager (char** path)
		 : head(0)
{
    for (int i = 0; i < MAX_SEARCHPATH+1; i++)
	searchPath[i] = path[i];

    PortAddress localPortAddr(MANAGER_PORT_LOCAL);
    localPort = new Port(localPortAddr);
}

Manager::~Manager ()
{
    ProcessTable* tmp = head;
    
    while (tmp)
    {
	tmp = head->next;
	terminateServer(*head);
	delete head;
	head = tmp;
    }
    
    if (localPort != NULL)
	delete localPort;
}

char* Manager::locateService ( const char* service ) const
{
    int index = 0;
    char* fullServiceName = (char*) 0;
    struct stat buf;
    size_t serviceSize = ::strlen(service);
    
    while (searchPath[index])
    {
	fullServiceName = ::new char[serviceSize + ::strlen(searchPath[index]) + 4];
	::strcpy(fullServiceName, searchPath[index]);
	::strcat(fullServiceName, "/");
	::strcat(fullServiceName, service);
	
	if (::stat(fullServiceName, &buf) == 0)
	    return fullServiceName;
    
	::delete [] fullServiceName;

	index++;
    }
    
    return 0;
}

Boolean Manager::Initiate ( const char* service, PortAddress& serverAddr,
			    const Uid& serverID,
			    const Uid& clientID, long seqNo,
			    ProtocolType protocol )
{
    ProcessTable::Opcode found = ProcessTable::NOT_FOUND;
    ProcessTable* prev = 0;
    ProcessTable* tmp = head;
    char* fullServiceName = (char*) 0;
    const char* protocolName = ((protocol == DGRAM) ? DGRAM_PROTOCOL : STREAM_PROTOCOL);

    while ((found == ProcessTable::NOT_FOUND) && (tmp != NULL))
    {
	found = tmp->isPresent(service, serverID, clientID, seqNo);
	
        if (found == ProcessTable::NOT_FOUND)
	{
	    prev = tmp;
	    tmp = tmp->next;
	}
    }

    if (found != ProcessTable::NOT_FOUND)
    {
	if (!isAlive(*tmp))
	{
	    if (prev)
		prev->next = tmp->next;
	    delete tmp;
	    found = ProcessTable::NOT_FOUND;
	}
    }

    if (found == ProcessTable::NOT_FOUND)
    {
	pid_t p_id;
	
	/* Check that server is available */

	fullServiceName = locateService(service);

	if (fullServiceName == 0)
	    return FALSE;

#ifndef WIN32
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
	    else
		if (p_id < 0)
		    ::perror("MANAGER: Initiate, sub-fork ");
		else
		    exit(0);

	    exit(1);
	}
	else
	{
	    if (p_id < 0)
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
	}
#else
	/*
	 * We don't need to wait for the child to exit.
	 */
	
	(void) _flushall();
	p_id = _spawnl(_P_NOWAIT, fullServiceName, service, protocolName, 0);

	if (p_id < 0)
	{
	    perror("MANAGER: spawn failed ");
	    return FALSE;
	}
#endif
			      
	PortAddress addr;
	Buffer      buff;
	int number = 1;
	Buffer* RecBuff[1];

	RecBuff[0] = &buff;

#ifdef DEBUG
	cout << "**Manager - waiting to receive bind request from server" << endl;
#endif

	if (localPort->receiveMessage(addr, RecBuff, number, MANAGER_WAIT_TIME) == Port::DONE)
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
	    
#ifdef WIN32
	    head->_protocolName = protocol;
#endif

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

Boolean Manager::processInformation (Buffer& buff)
{
    Boolean result = FALSE;
    ProcessTable* tmp = head;
    int count = 0;

    while (tmp != 0)
    {
	count++;
	tmp = tmp->next;
    }

    result = buff.pack(count);

    if (result && (count > 0))
    {
	tmp = head;

	while ((tmp != 0) && (result))
	{
	    result = tmp->packInto(buff);

	    tmp = tmp->next;
	}
    }

    return result;
}

#ifdef WIN32

/*
 * NT will not let us send a kill signal to the server, so we have to convert
 * this into a message. This is ok, unless we get into an infinite loop at the
 * server, and so cannot receive messages!
 */

void Manager::tcpTerminate (const ProcessTable& serverId) const
{
    ReliablePort port;
    Buffer buff1, buff2;
    Buffer *sendBuff[1];
    Buffer *recBuff[1];
    int number = 1;

    buff1.pack((int) DO_TERMINATE);
    sendBuff[0] = &buff1;
    recBuff[0] = &buff2;

    if (port.setupConnection(serverId.address) == ReliablePort::DONE)
    {
	if (port.sendMessage(sendBuff, 1) == ReliablePort::DONE)
	{
	    /*
	     * We should probably check the return message.
	     */

	    if (port.receiveMessage(recBuff, number) != ReliablePort::DONE)
		cerr << "Manager::tcpTerminate receive error." << endl;
	}
	else
	    cerr << "Manager::tcpTerminate send error." << endl;

	port.endConnection();
    }
    else
	cerr << "Manager::tcpTerminate setupConnection error." << endl;
}

void Manager::udpTerminate (const ProcessTable& serverId) const
{
    Port port;
    Buffer buff1, buff2;
    Buffer *sendBuff[1];
    Buffer *recBuff[1];
    int number = 1;
    PortAddress dummy;

    buff1.pack((int) DO_TERMINATE);
    sendBuff[0] = &buff1;
    recBuff[0] = &buff2;

    if (port.sendMessage(serverId.address, sendBuff, 1) == Port::DONE)
    {
	if (port.receiveMessage(dummy, recBuff, number) != Port::DONE)
	    cerr << "Manager::udpTerminate receive error." << endl;
    }
    else
	cerr << "Manager::udpTerminate send error." << endl;
}

#endif
	
void Manager::terminateServer (const ProcessTable& serverId) const
{
#ifndef WIN32
    ::kill(serverId.pid, SIGTERM);
#else
    if (serverId._protocolName == DGRAM)
	udpTerminate(serverId);
    else
	tcpTerminate(serverId);		
#endif
}

Boolean Manager::isAlive (const ProcessTable& serverId) const
{
#ifndef WIN32
    if (::kill(serverId.pid, 0) == 0)
	return TRUE;
    else
	return FALSE;
#else
    return TRUE;  // eventually send a message
#endif    
}

/*
 * NT does not really have any concept of sending signals to other
 * processes, so we simply send the process a message as though
 * we were a client.
 */

void Manager::Terminate ( const char* service, const Uid& serverID,
			  const Uid& clientID, long seqNo )
{
    ProcessTable::Opcode found = ProcessTable::NOT_FOUND;
    ProcessTable* tmp = head;
    ProcessTable* trail = 0;

#ifdef DEBUG
    cout << "**Manager - request to terminate " << service << " and " << serverID << endl;
#endif
    
    while ((found == ProcessTable::NOT_FOUND) && (tmp != 0))
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
	    terminateServer(*tmp);

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

