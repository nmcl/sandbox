/*
 * Copyright (C) 1995-1997,
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

#include <CosTransactions/OTS.h>
#include <CosTransactions/OTS_Current.h>

#include <PortAddress.h>
#include <RelPort.h>

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
int res = -1;
Bank* bank = 0;
const char* userId = "MarkLittle";

void signal_handler (int dummy DOTDOTDOT)
{
    if (bank)
    {
	delete bank;
	bank = 0;
    }

    ::signal(SIGINT, signal_handler);
    ::signal(SIGQUIT, signal_handler);
    ::signal(SIGTERM, signal_handler);    
}

void insert (int pinNumber, int amount, ReliablePort& port)
{
    Uid gUid;
    OTS_Current& current = OTS::current();
    
    current.begin();

    Bank::Outcome outcome = bank->insert(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, userId, port);
    Control_ptr cont = current.get_control();

    if (!CORBA_(is_nil)(cont))
    {
	Coordinator_ptr coord = cont->get_coordinator();

	if (!CORBA_(is_nil)(coord))
	{
	    coord->register_resource(jr);

	    CORBA_(release)(coord);
	}

	CORBA_(release)(cont);
    }

    if (outcome == Bank::DONE)
	current.commit(0);
    else
	current.rollback();

    CORBA_(release)(jr);
}

void transfer (int pinFrom, int pinTo, int amount, ReliablePort& port)
{
    Uid gUid;
    OTS_Current& current = OTS::current();
    
    current.begin();
    
    Bank::Outcome outcome = bank->transfer(pinFrom, pinTo, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, userId, port);
    Control_ptr cont = current.get_control();

    if (!CORBA_(is_nil)(cont))
    {
	Coordinator_ptr coord = cont->get_coordinator();

	if (!CORBA_(is_nil)(coord))
	{
	    coord->register_resource(jr);

	    CORBA_(release)(coord);
	}

	CORBA_(release)(cont);
    }

    if (outcome == Bank::DONE)
	current.commit(0);
    else
	current.rollback();

    CORBA_(release)(jr);
}

void withdraw (int pinNumber, int amount, ReliablePort& port)
{
    Uid gUid;
    OTS_Current& current = OTS::current();
    
    current.begin();

    Bank::Outcome outcome = bank->withdraw(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, userId, port);
    Control_ptr cont = current.get_control();

    if (!CORBA_(is_nil)(cont))
    {
	Coordinator_ptr coord = cont->get_coordinator();

	if (!CORBA_(is_nil)(coord))
	{
	    coord->register_resource(jr);

	    CORBA_(release)(coord);
	}

	CORBA_(release)(cont);
    }

    if (outcome == Bank::DONE)
	current.commit(0);
    else
	current.rollback();

    CORBA_(release)(jr);
}

void inspect (int pinNumber, int amount, ReliablePort& port)
{
    Uid gUid;
    OTS_Current& current = OTS::current();
    
    current.begin();
    
    Bank::Outcome outcome = bank->inspect(pinNumber, amount);
    JavaRecord *jr = new JavaRecord(gUid, amount, outcome, userId, port);
    Control_ptr cont = current.get_control();

    if (!CORBA_(is_nil)(cont))
    {
	Coordinator_ptr coord = cont->get_coordinator();

	if (!CORBA_(is_nil)(coord))
	{
	    coord->register_resource(jr);

	    CORBA_(release)(coord);
	}

	CORBA_(release)(cont);
    }

    if (outcome == Bank::DONE)
	current.commit(0);
    else
	current.rollback();

    CORBA_(release)(jr);
}

/*
 * Each operation is performed as a separate top-level atomic action.
 * Replies from the client to the browser are performed by the Java
 * abstract record.
 */

void doWork (Buffer& request, ReliablePort& port)
{
    int workId = 0;
    int amount = 0;
    int pinNumber = 0;
    Boolean res = request.unpack(workId);

    if (!res)
    {
	cout << "\nBank received an illegal message." << endl;
	return;
    }

    cout << "\nBank received request. Processing ... ";
    
    if (request.unpack(pinNumber))
    {
	switch (workId)
	{
	case OP_INSERT:
	    {
		cout << "Received insert operation." << endl;
		cout << "Starting atomic action." << endl;
		
		if (request.unpack(amount))
		    insert(pinNumber, amount, port);
		else
		    return;
	    }
	break;
	case OP_WITHDRAW:
	    {
		cout << "Received withdraw operation." << endl;
		cout << "Starting atomic action." << endl;

		if (request.unpack(amount))
		    withdraw(pinNumber, amount, port);
		else
		    return;
	    }
	break;
	case OP_INSPECT:
	    {
		cout << "Received inspect operation." << endl;
		cout << "Starting atomic action." << endl;
		
		inspect(pinNumber, amount, port);
	    }
	break;
	case OP_TRANSFER:
	    {
		cout << "Received transfer operation." << endl;
		cout << "Starting atomic action." << endl;
		
		int pinTo;

		if (request.unpack(pinTo) && request.unpack(amount))
		    transfer(pinNumber, pinTo, amount, port);
		else
		    return;
	    }
	break;
	default:
	    cerr << "Opcode error." << endl;
	    break;
	}
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

    PortAddress addr(ArjunaPort), from;
    ReliablePort port(addr);

    for (;;)
    {
	if (port.acceptConnection(from) == ReliablePort::DONE)
	{
	    /*
	     * Get work and deal with it. Then go back to receive
	     * further work.
	     */

	    Buffer buff;
	    Buffer* work[1];
	    int size = 1;

	    work[0] = &buff;

	    if (port.receiveMessage(work, size) == ReliablePort::DONE)
	    {
		doWork(buff, port);
		port.breakConnection();
	    }
	}
    }

    return -1;
}

	    
		
