/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferServer.cc,v 1.4 1996/02/08 14:13:41 ngdp Exp $
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


const long DO_WORK = 1;


int main (int argc, char** argv)
{
#ifdef DEBUG
    cout << "BufferServer started" << endl;
#endif
    
    SimpleRpcServer server;
    Buffer *work[2];
    int number = 0;
    long opcode = 0;

    server.initialise(argc, argv);
    
    for (;;)
    {
	work[0] = new Buffer;
	work[1] = new Buffer;
	
	opcode = 0;

#ifdef DEBUG
	cout << "BufferServer - waiting for work" << endl;
#endif
	
	server.getWork(work, number);

#ifdef DEBUG
	cout << "BufferServer - received work" << endl;
#endif
	
	work[0]->unpack(opcode);
	
	if (opcode == DO_WORK)
	{
	    work[0]->rewrite();
	    work[0]->pack(0);

#ifdef DEBUG
	    cout << "BufferServer - sending reply" << endl;
#endif
	    
	    server.sendResult(work, 2);
	}
	
	if (work[0])
	    delete work[0];
	if (work[1])
	    delete work[1];

    }

    return 0;
    
}
