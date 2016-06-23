/*
 * Copyright 1990-1997,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 */

#ifdef WIN32
#  include <System/direct.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef SERVERCONNECTION_H_
#  include <StubGen/ServerConnection.h>
#endif

ServerConnection::ServerConnection (SimpleServerConnection* conn)
				   : _conn(conn)
{
}

ServerConnection::~ServerConnection ()
{
    if (_conn)
	delete _conn;
}

void ServerConnection::sendResult ( Int32 errcode, Buffer& result )
{
    Buffer errBuff;
    Buffer* recBuff[2];
    int number = 2;
    SimpleServerConnection::Outcome res;

    errBuff.pack(errcode);
    
    recBuff[0] = &errBuff;
    recBuff[1] = &result;

    res = _conn->sendResult(recBuff, number);
}
