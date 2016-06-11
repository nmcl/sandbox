/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <System/iostream.h>
#include <System/unistd.h>
#include <System/limits.h>
#include <System/netdb.h>
#include <System/stdlib.h>
#include <System/sys/socket.h>
#include <System/netinet/in.h>
#include <System/sys/param.h>
#include <System/string.h>
#include <System/signal.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef BANK_H_
#  include "Bank.h"
#endif

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

#ifndef JAVAR_H_
#  include "JavaR.h"
#endif

unsigned short ArjunaPort = 5010;

AtomicAction* globalAction = 0;
const TypeName tn = "JavaRecord";
int res = -1, s = -1;
Bank* bank = 0;
const char* userId = "MarkLittle";


void signal_handler (int dummy DOTDOTDOT)
{
    cout << "in signal handler" << endl;
    
    if (s != -1)
	close(s);

    if (globalAction)
    {
	globalAction->Abort();
	delete globalAction;
	globalAction = 0;
    }
    
    if (bank)
    {
	delete bank;
	bank = 0;
    }
}


void insert (long accountNumber, long amount, int msgSock)
{
    Uid gUid;
    
    globalAction = new AtomicAction;
    globalAction->Begin();

    JavaRecord *jr = new JavaRecord(gUid, tn, amount, userId, msgSock);
    if (AtomicAction::Current()->add(jr) == AtomicAction::ADDED)
    {
	if (bank->insert(accountNumber, amount))
	    globalAction->End();
	else
	    globalAction->Abort();
    }
    else
    {
	delete jr;
	globalAction->Abort();
    }

    delete globalAction;
    globalAction = 0;
}

void withdraw (long accountNumber, long amount, int msgSock)
{
    Uid gUid;
    
    globalAction = new AtomicAction;
    globalAction->Begin();

    JavaRecord *jr = new JavaRecord(gUid, tn, amount, userId, msgSock);
    if (AtomicAction::Current()->add(jr) == AtomicAction::ADDED)
    {
	if (bank->withdraw(accountNumber, amount))
	    globalAction->End();
	else
	    globalAction->Abort();
    }
    else
    {
	delete jr;
	globalAction->Abort();
    }

    delete globalAction;
    globalAction = 0;
}

void inspect (long accountNumber, long amount, int msgSock)
{
    Uid gUid;
    
    globalAction = new AtomicAction;
    globalAction->Begin();
	    
    if (bank->inspect(accountNumber, amount))
    {
	cout << "account " << accountNumber << " has " << amount << endl;
	
	JavaRecord *jr = new JavaRecord(gUid, tn, amount, userId, msgSock);
	if (AtomicAction::Current()->add(jr) == AtomicAction::ADDED)
	    globalAction->End();
	else
	{
	    delete jr;
	    globalAction->Abort();
	}
    }
    else
	globalAction->Abort();
    
    delete globalAction;
    globalAction = 0;
}

/*
 * Each operation is performed as a separate top-level atomic action.
 * Replies from the client to the browser are performed by the Java
 * abstract record.
 */

void doWork (char* request, int msgSock)
{
    long workId = atol(request), accountNumber, amount = 0;
    char* ptr = strchr(request, ' ');

    if (!ptr)
	return;

    ptr++;
    accountNumber = atol(ptr);

    cout << "accountNumber " << accountNumber << endl;
    
    switch (workId)
    {
    case OP_INSERT:
	{
	    if (!globalAction)
	    {
		ptr = strchr(ptr, ' ');
		if (!ptr)
		    return;
		
		ptr++;
		amount = atol(ptr);
		insert(accountNumber, amount, msgSock);
	    }
	}
	break;
    case OP_WITHDRAW:
	{
	    if (!globalAction)
	    {
		ptr = strchr(ptr, ' ');
		if (!ptr)
		    return;
		
		ptr++;
		amount = atol(ptr);
		withdraw(accountNumber, amount, msgSock);
	    }
	}
	break;
    case OP_INSPECT:
	{
	    if (!globalAction)
		inspect(accountNumber, amount, msgSock);
	}
	break;
    default:
	cerr << "Opcode error." << endl;
	break;
    }
}


int main (int, char**)
{
    ::signal(SIGINT, signal_handler);
    ::signal(SIGQUIT, signal_handler);
    ::signal(SIGTERM, signal_handler);
    
    //    Uid bankId("80f0960b:812:30c5bdbe:3");
    
    //    bank = new Bank(res, bankId);
    bank = new Bank(res);

    if (res == -1)
    {
	cerr << "Bank creation error." << endl;
	return -1;
    }
    
    struct sockaddr_in sockAddr;
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s >= 0)
    {
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(ArjunaPort);

	if (bind(s, (struct sockaddr*) &sockAddr, sizeof(sockAddr)) >= 0)
	{
	    listen(s, 5);
	    
	    for (;;)
	    {
		struct sockaddr_in from;
		int length = sizeof(struct sockaddr);
		int msgSock = ::accept(s, (struct sockaddr*) &from, &length);

		cout << "accepted" << endl;
		
		if (msgSock >= 0)
		{
		    char request[REQUEST_SIZE];
		    int length = REQUEST_SIZE;

		    ::memset(request, '\0', length);

		    /*
		     * Get work and deal with it. Then go back to receive
		     * further work.
		     */
		    
		    if (recv(msgSock, request, length, 0) > 0)
			doWork(request, msgSock);

		    ::close(msgSock);
		}
 	    }
	}
	else
	    cout << "Bind error." << endl;
    }
    else
	cout << "Socket error." << endl;

    return -1;
}

	    
		
