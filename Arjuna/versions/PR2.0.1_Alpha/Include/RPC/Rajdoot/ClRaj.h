/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj.h,v 1.1 1993/11/03 14:30:56 nmcl Exp $
 */

#ifndef RAJDOOT_CLRAJ_H_
#define RAJDOOT_CLRAJ_H_

/*
 * This class provides C++ interface to the client side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

class ArjunaName;
class Buffer;

#ifndef CLSTATUS_H_
#   include <RPC/ClStatus.h>
#endif

#define BASE_SERVER		0

#define DEFAULT_NUMBEROFSERVERS 1
#define DEFAULT_RETRY       	2
#define DEFAULT_TIMEOUT     	1000

#define MINIMUM_RETRY       	1
#define MINIMUM_TIMEOUT     	100


//  @NoRemote, @NoMarshall
class ClientRajdoot_rpc
{
public:

//
//    	Fundermental generic operations provided by
//	the RPC interface.
//

    	ClientRajdoot_rpc(ArjunaName* ArjNam);
    	ClientRajdoot_rpc(const char* serviceName = NULL);
    	virtual ~ClientRajdoot_rpc();

    	RPC_Status 		Initiate();

    	RPC_Status 		Call(long  opcode,  Buffer& call,
				     long& errcode, Buffer& result);

    	virtual RPC_Status 	Terminate();

//
//    	Additional operations provided by the RPC interface.
//

  	ServerStatus	GetServerStatus();

    	RPC_Status 	Call(long  opcode,  Buffer& call1,   Buffer& call2,
			     long& errcode, Buffer& result1, Buffer& result2);

    	RPC_Status 	SetAllToDefault();
    	RPC_Status 	SetToDefault(int serverNum = BASE_SERVER);

    	RPC_Status 	SetNumberOfServers(int numOfServers = DEFAULT_NUMBEROFSERVERS);
    	RPC_Status 	GetNumberOfServers(int& numOfServers);

    	RPC_Status 	SetServiceName(char* service = NULL, int serverNum = BASE_SERVER);
    	RPC_Status 	GetServiceName(char*& serviceName, int serverNum = BASE_SERVER);

    	RPC_Status 	SetHost(char* hostName = NULL, int serverNum = BASE_SERVER);
    	RPC_Status 	GetHost(char*& hostName, int serverNum = BASE_SERVER);

    	RPC_Status 	SetTimeout(long  tout = DEFAULT_TIMEOUT);
    	RPC_Status 	GetTimeout(long& tout);

    	RPC_Status 	SetRetry(long  rty = DEFAULT_RETRY);
    	RPC_Status 	GetRetry(long& rty);

        RPC_Status      SetObjectUid (UNIQUEID rUid);

        RPC_Status      SetBurstSize (unsigned long);
        RPC_Status      GetBurstSize (unsigned long&) const;
    
        RPC_Status      SetBurstTime (unsigned long);
        RPC_Status      GetBurstTime (unsigned long&) const;
    

	Boolean		Ping();	     // Not yet implemented (return TRUE).

//
//    	Additional operations provided by the RPC interface,
//	to allow the uses of atomic actions.
//

	ClientRajdoot_rpc(ClientRajdoot_rpc* doner);

protected:

    	RPC_Status 	SoftTerminate();

private:

//      Private Variables

	ServerStatus	status;

	int		numOfServers;
    	long         	retry, timeout;
    	OBJECT_DESC  	*objects;
	char		*defaultServiceName;

    	GROUPS       	*callGroup;
    	MID          	actionId;
    	FILTER_INFO  	callFilter;
    	FILTER_INFO  	actionGroupFilter;
};

#endif
