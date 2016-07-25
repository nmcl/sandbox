/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.cc,v 1.6 1993/06/25 14:17:13 nmcl Exp $
 */

#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>


#ifdef DEBUG
#include <Common/Debug.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#include <RPC/ArjServers/NSInterface.h>

#include <Configure.h>

#include "ServerList.h"

ServerList::ServerList()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::ServerList()\n";
#endif

    servers = NULL;
    user_server = NULL;
    number_of_servers = 0;
    ScanNameServer();
}


ServerList::~ServerList()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::~ServerList()\n";
#endif

    Clear();
}


void ServerList::Clear()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::Clear()\n";
#endif

    ListElement *tmp;

    if (user_server != NULL)
    {
	delete user_server;
	user_server = NULL;
    }

    while (servers != NULL)
    {
	tmp = servers->next;
	delete servers->server;
	delete servers;
	servers = tmp;
    }
    number_of_servers = 0;
}


int ServerList::Count()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::Count()\n";
#endif

    return number_of_servers;
}


void ServerList::ScanNameServer()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::ScanNameServer()\n";
#endif

    Clear();
    struct passwd *user_pwentry = getpwuid(getuid());

    Boolean done = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
    debug_stream << "ServerList::ScanNameServer -> name server (servicename) = (" << NAME_SERVER_SERVICE_Q << ")\n";
#endif

    int res = -1;
    NSInterface *NS = new NSInterface(res);
    if (res == 0)
	done = TRUE;

    if (done)
    {
	Boolean finished, eod;
	AtomicAction A;

	A.Begin();

	NS->restart(done);
	finished = FALSE;
	while (! finished)
	{
	    Buffer Key;
	    NS->getNextName(done, Key, eod);
	    if (! eod)
	    {
		if (done)
		{
		    Buffer Data;
		    NS->fetchName(done, Key,Data);
		    if (done)
		    {
			ArjunaName *ANServer = new ArjunaName;
			done = ANServer->unpack(Data);
			if (done)
			{
			    if (strcmp(ANServer->GetRefServiceName(), "Calendar") == 0)
			    {
				if (strcmp(ANServer->GetRefObjName(), user_pwentry->pw_name) == 0)
				{
#ifdef DEBUG
				    debug_stream << FUNCTIONS << FAC_USER2 << VIS_PUBLIC;
				    debug_stream << "ScanNameServer -> found user's own server\n";
#endif				    
				    user_server = ANServer;    // user's own Calendar server
				}
				else
				    Add(ANServer);             // somebody else's Calendar server

			    }
			}
		    }
		}
	    }
	    finished = (Boolean) ((!done) || eod);
	}

	A.End();
    }

    delete NS;
}


void ServerList::Delete(int index)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::Delete(int index)\n";
#endif

    int i;
    ListElement *curr, *prev;

    if (index < number_of_servers)
    {
	i = 0;
	prev = NULL;
	curr = servers;
	while ((i != index) && (curr != NULL))
	{
	    prev = curr;
	    curr = curr->next;
	    i++;
	}
	if (curr != NULL)
	{
	    if (prev == NULL)
	    {
		// delete first item from list
		servers = curr->next;
		delete curr->server;
		delete curr;
	    }
	    else
	    {
		// delete item from somewhere inside list
		prev->next = curr->next;
		delete curr->server;
		delete curr;
	    }
	    number_of_servers--;
	}
    }
}


void ServerList::Add(ArjunaName *ServerName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::Add(ArjunaName *ServerName)\n";
#endif

    ListElement *new_element = new ListElement;
    new_element->server = ServerName;
    new_element->next = servers;
    servers = new_element;
    number_of_servers++;
}


ArjunaName *ServerList::Get(int index)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::Get(int index)\n";
#endif

    int i;
    ListElement *tmp;

    if (index < number_of_servers)
    {
	i = 0;
	tmp = servers;
	while ((i != index) && (tmp != NULL))
	{
	    tmp = tmp->next;
	    i++;
	}
	if (tmp == NULL)
	    return NULL;
	else
	    return tmp->server;
    }
    else
	return NULL;
}


ArjunaName *ServerList::GetMyServer()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::GetMyServer()\n";
#endif

    return user_server;
}
