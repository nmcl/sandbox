/*
 * Copyright (C) 1995-1997, 1998,
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

#include <Arjuna/AtomicA.h>
#include <Thread/Thread.h>

#include <PortAddress.h>
#include <TCPConnection.h>
#include <RelPort.h>

#ifndef BANK_H_
#  include "Bank.h"
#endif

#ifndef JAVAR_H_
#  include "JavaR.h"
#endif

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

static const int poolSize = 10;

unsigned short ArjunaPort = 5010;
Bank* bank = NULL;
Thread* threadPool[poolSize];

void signal_handler (int dummy DOTDOTDOT)
{
    if (bank)
    {
	delete bank;
	bank = 0;
    }

    for (int j = 0; j < poolSize; j++)
    {
	if (threadPool[j])
	{
	    delete threadPool[j];
	    threadPool[j] = 0;
	}
    }
    
    exit(0);
}

void insert (int pinNumber, int amount, TCPConnection* port)
{
    Uid gUid;
    AtomicAction current;    
    
    current.begin();

    Bank::Outcome outcome = bank->insert(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, port);

    current.add(jr);

    if (outcome == Bank::DONE)
	current.commit();
    else
	current.abort();
}

void transfer (int pinFrom, int pinTo, int amount, TCPConnection* port)
{
    Uid gUid;
    AtomicAction current;    
    
    current.begin();
    
    Bank::Outcome outcome = bank->transfer(pinFrom, pinTo, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, port);

    current.add(jr);
    
    if (outcome == Bank::DONE)
	current.commit();
    else
	current.abort();
}

void withdraw (int pinNumber, int amount, TCPConnection* port)
{
    Uid gUid;
    AtomicAction current;    
    
    current.begin();

    Bank::Outcome outcome = bank->withdraw(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, port);

    current.add(jr);
    
    if (outcome == Bank::DONE)
	current.commit();
    else
	current.abort();
}

void inspect (int pinNumber, int amount, TCPConnection* port)
{
    Uid gUid;
    AtomicAction current;    
    
    current.begin();
    
    Bank::Outcome outcome = bank->inspect(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, port);

    current.add(jr);

    if (outcome == Bank::DONE)
	current.commit();
    else
	current.abort();
}

void recover (char* uid, TCPConnection* port)
{
    Uid gUid(uid, TRUE);  // errors in Uid shouldn't cause us to exit!
    AtomicAction current;    

    current.begin();
    
    JavaRecord *jr = new JavaRecord(gUid, port);

    current.add(jr);
    
    current.commit();
}

/*
 * Each operation is performed as a separate top-level atomic action.
 * Replies from the client to the browser are performed by the Java
 * abstract record.
 */

void* doWork (void* param)
{
    if (!param)
    {
	cerr << "Bank thread has no work!" << endl;
	
	return 0;
    }
    
    /*
     * Get work and deal with it. Then go back to receive
     * further work.
     */

    Buffer request;
    Buffer* work[1];
    int size = 1;
    int tmpId = 0;
    int amount = 0;
    int pinNumber = 0;    
    TCPConnection* connection = (TCPConnection*) param;
    
    work[0] = &request;

    connection->receiveMessage(work, size);

    Thread::yield();

    Boolean res = request.unpack(tmpId);

    if (!res)
    {
	cout << "\nBank received an illegal message." << endl;
    }
    else
    {
	Opcode workId = (Opcode) tmpId;
    
	cout << "\nBank received request. Processing ... ";

	if (workId != OP_RECOVER)
	    res = request.unpack(pinNumber);

	Thread::yield();
	
	if (res)
	{
	    switch (workId)
	    {
	    case OP_INSERT:
		{
		    cout << "Received insert operation for " << pinNumber << endl;
		    cout << "Starting atomic action." << endl;
		
		    if (request.unpack(amount))
			insert(pinNumber, amount, connection);
		}
	    break;
	    case OP_WITHDRAW:
		{
		    cout << "Received withdraw operation for " << pinNumber << endl;
		    cout << "Starting atomic action." << endl;

		    if (request.unpack(amount))
			withdraw(pinNumber, amount, connection);
		}
	    break;
	    case OP_INSPECT:
		{
		    cout << "Received inspect operation for " << pinNumber << endl;
		    cout << "Starting atomic action." << endl;
		
		    inspect(pinNumber, amount, connection);
		}
	    break;
	    case OP_TRANSFER:
		{
		    int pinTo;

		    if (request.unpack(pinTo) && request.unpack(amount))
		    {
			cout << "Received transfer operation for " << pinNumber << " and " << pinTo << endl;
			cout << "Starting atomic action." << endl;
			
			transfer(pinNumber, pinTo, amount, connection);
		    }
		}
	    break;
	    case OP_RECOVER:
		{
		    cout << "Received transaction recovery operation." << endl;
		    cout << "Determining transaction outcome." << endl;

		    char* uid = (char*) 0;

		    if (request.unpack(uid))
		    {
			recover(uid, connection);
		    }

		    if (uid)
			::delete [] uid;
		}
	    break;
	    default:
		cerr << "Opcode error." << endl;
		break;
	    }
	}
    }

    delete connection;

    return 0;
}


int main (int argc, char** argv)
{
    for (int j = 0; j < argc; j++)
    {
	if (::strcmp(argv[j], "-port") == 0)
	    ArjunaPort = ::atoi(argv[j+1]);
	if (::strcmp(argv[j], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-port <port>] [-help]" << endl;
	    return 0;
	}
    }
    
    ::signal(SIGINT, signal_handler);
    ::signal(SIGQUIT, signal_handler);
    ::signal(SIGTERM, signal_handler);

    int res = -1;
    
    bank = new Bank(res);

    if (res == -1)
    {
	cerr << "Bank creation error." << endl;
	return -1;
    }

    for (int i = 0; i < poolSize; i++)
    {
	threadPool[i] = 0;
    }
    
    PortAddress addr(ArjunaPort), from;
    ReliablePort port(addr);
    
    for (;;)
    {
	/*
	 * Find a spare slot in the thread pool before accepting any connection.
	 */
	
	int index = 0;
	Boolean found = FALSE;
	
	for (index = 0; (index < poolSize) && !found; index++)
	{
	    if (threadPool[index] == 0)
	    {
		found = TRUE;
		break;
	    }
	    else
	    {
		if (threadPool[index]->status() == Thread::THR_IS_TERMINATED)
		{
		    delete threadPool[index];
		    threadPool[index] = 0;
		    found = TRUE;
		    break;
		}
	    }
	}

	if (found)
	{
	    TCPConnection* connection = port.acceptConnection();
	    Thread* t = new Thread(doWork, connection);

	    threadPool[index] = t;
	    
	    t->resume();
	}
	else
	    cerr << "WARNING: thread pool exhausted." << endl;

	Thread::yield();
    }

    for (j = 0; j < poolSize; j++)
    {
	if (threadPool[j])
	{
	    delete threadPool[j];
	    threadPool[j] = 0;
	}
    }

    if (bank)
	delete bank;
    
    return 0;
}
