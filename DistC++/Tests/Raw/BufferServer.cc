/*
 * Copyright (C) 1993-1997, 1998,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferServer.cc,v 1.2 1998/11/20 08:28:57 nmcl Exp $
 */

#include <Config/Configure.h>

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
# include <System/iostream.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

#ifndef SIMPLESERVERCONNECTION_H_
#  include <RPC/SimpleRpc/SimpleServerConnection.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

const long DO_WORK = 1;

struct WorkData
{
    SimpleServerConnection* connection;
    Buffer* buffer1;
    Buffer* buffer2;
};


void* func (void* param)
{
    long opcode = 0;
    WorkData* data = (WorkData*) param;
    Buffer* work[2];

    work[0] = data->buffer1;
    work[1] = data->buffer2;

    data->buffer1->unpack(opcode);
	
    if (opcode == DO_WORK)
    {
	data->buffer1->rewrite();
	data->buffer1->pack(0);

	//#ifdef DEBUG
	cout << "BufferServer - sending reply" << endl;
	//#endif
	    
	data->connection->sendResult(work, 2);
    }
    else
	cerr << "Received unknown opcode " << opcode << endl;

    delete data->connection;
    delete data->buffer1;
    delete data->buffer2;
    delete data;
    
    Thread* curr = Thread::current();
    delete curr;  // ok as long as we don't do anything after this!
    
    return 0;
}


int main (int argc, char** argv)
{
    //#ifdef DEBUG
    cout << "BufferServer started" << endl;
    //#endif

    SimpleRpcServer server;
    Buffer* work[2];
    int number = 2;

    /*
     * Initialise the server object.
     */
    
    server.initialise(argc, argv);
    
    for (;;)
    {
	number = 2;
	
	work[0] = new Buffer;
	work[1] = new Buffer;

#ifdef DEBUG
	cout << "BufferServer - waiting for work" << endl;
#endif

	SimpleServerConnection* connection = server.getWork(work, number);

#ifdef DEBUG
	cout << "BufferServer - received work" << endl;
#endif

	WorkData* data = new WorkData;

	data->connection = connection;
	data->buffer1 = work[0];
	data->buffer2 = work[1];

	Thread* t = new Thread(func, data);
	t->resume();

	Thread::yield();
    }
    
    return 0;
}
