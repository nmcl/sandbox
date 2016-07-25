/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef ANSA_CLANSA_H_
#define ANSA_CLANSA_H_

/*
 * $Id: ClANSA.h,v 1.1 1993/11/03 14:30:24 nmcl Exp $
 *
 * This class provides C++ interface to the client side of the
 * ANSA rpc protocol.
 *
 */

struct ObjectInfo;

class ArjunaName;
class Buffer;

/**********************************************************************/
/* these should really be defined on the ANSA side of things, but ... */
/**********************************************************************/
#define MAX_HOSTNAME_SIZE 64
#define MAX_SERVICENAME_SIZE 32

#define BASE_SERVER 0
#define DEFAULT_NUMBEROFSERVERS 1

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CLSTATUS_H_
#   include <RPC/ClStatus.h>
#endif

class ClientANSA_rpc
{
public:

// Fundamental operations provided by the RPC interface

    ClientANSA_rpc(const char* serviceName = NULL);
    ClientANSA_rpc(ArjunaName* ArjNam);
    ~ClientANSA_rpc();

    RPC_Status         Initiate();
    RPC_Status         Call(long opcode, Buffer& call, long& errcode, Buffer& result);
    virtual RPC_Status Terminate();

// Additional operations provided by the RPC interface

    ServerStatus GetServerStatus();

    RPC_Status   Call(long opcode, Buffer& call1, Buffer& call2,
		      long& errcode, Buffer& result1, Buffer& result2);

    RPC_Status   SetAllToDefault();
    RPC_Status   SetToDefault(int serverNum = BASE_SERVER);

    RPC_Status   SetNumberOfServers(int numOfServers = DEFAULT_NUMBEROFSERVERS);
    RPC_Status   GetNumberOfServers(int& numberOfServers);

    RPC_Status   SetServiceName(char* service = NULL, int serverNum = BASE_SERVER);
    RPC_Status   GetServiceName(char*& service, int serverNum = BASE_SERVER);

    RPC_Status   SetHost(char* hostName = NULL, int serverNum = BASE_SERVER);
    RPC_Status   GetHost(char*& hostName, int serverNum = BASE_SERVER);

    ClientANSA_rpc(ClientANSA_rpc* doner);

protected:
    RPC_Status   SoftTerminate();

private:
    ServerStatus status;

    int                numOfServers;
    struct ObjectInfo  *objects;
    char               *defaultServiceName;
};

#endif
