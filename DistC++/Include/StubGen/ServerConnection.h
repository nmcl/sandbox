/*
 * Copyright 1990-1997,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

#ifndef SERVERCONNECTION_H_
#define SERVERCONNECTION_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef SIMPLESERVERCONNECTION_H_
#  include <RPC/SimpleRpc/SimpleServerConnection.h>
#endif

class ServerConnection
{
public:
    ServerConnection (SimpleServerConnection*);
    virtual ~ServerConnection ();

    void sendResult (Int32 errcode, Buffer& result);    
    
private:
    SimpleServerConnection* _conn;
};

#endif
