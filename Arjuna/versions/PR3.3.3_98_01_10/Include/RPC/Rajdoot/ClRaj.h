/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRaj.h,v 1.10.6.1 1996/10/10 12:25:51 ngdp Exp $
 */

#ifndef RAJDOOT_CLRAJ_H_
#define RAJDOOT_CLRAJ_H_

/*
 * This class provides C++ interface to the client side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

class ArjunaName;
class Buffer;

#ifndef CLSTATUS_H_
#   include <RPC/ClStatus.h>
#endif

/*
 * The time per call is proportional to the timeout/retries.
 * Note: the timeouts given here are in CENTISECONDS.
 */

#define BASE_SERVER		0

#define DEFAULT_NUMBEROFSERVERS 1
#define DEFAULT_RETRY       	2
#define DEFAULT_TIMEOUT     	2000

#define MINIMUM_RETRY       	1
#define MINIMUM_TIMEOUT     	100

#define MAXIMUM_TIMEOUT         200000

//  @NoRemote, @NoMarshall
class ClientRajdoot_rpc
{
public:

//
//    	Fundermental generic operations provided by
//	the RPC interface.
//

    	ClientRajdoot_rpc (ArjunaName* ArjNam);
    	ClientRajdoot_rpc (const char* serviceName = NULL);
    	virtual ~ClientRajdoot_rpc ();

    	RPC_Status 		initiate ();

    	RPC_Status 		call (Int32  opcode,  Buffer& call,
				      Int32& errcode, Buffer& result);

    	virtual RPC_Status 	terminate ();

//
//    	Additional operations provided by the RPC interface.
//

  	ServerStatus	getServerStatus ();

    	RPC_Status 	call (Int32  opcode,  Buffer& call1,   Buffer& call2,
			      Int32& errcode, Buffer& result1, Buffer& result2);

    	RPC_Status 	setAllToDefault ();
    	RPC_Status 	setToDefault (int serverNum = BASE_SERVER);

    	RPC_Status 	setNumberOfServers (int numOfServers = DEFAULT_NUMBEROFSERVERS);
    	RPC_Status 	getNumberOfServers (int& numOfServers);

    	RPC_Status 	setServiceName (const char* const service = NULL,
					int serverNum = BASE_SERVER);
    	RPC_Status 	getServiceName (char*& serviceName,
					int serverNum = BASE_SERVER);

    	RPC_Status 	setHost (const char* const hostName = NULL,
				 int serverNum = BASE_SERVER);
    	RPC_Status 	getHost (char*& hostName, int serverNum = BASE_SERVER);

    	RPC_Status 	setTimeout (Int32  tout = DEFAULT_TIMEOUT);
    	RPC_Status 	getTimeout (Int32& tout);

    	RPC_Status 	setRetry (Int32  rty = DEFAULT_RETRY);
    	RPC_Status 	getRetry (Int32& rty);

        RPC_Status      setObjectUid (UNIQUEID rUid);

        RPC_Status      setBurstSize (Uint32 = 0);
        RPC_Status      getBurstSize (Uint32&) const;
    
        RPC_Status      setBurstTime (Uint32 = 0);
        RPC_Status      getBurstTime (Uint32&) const;

        RPC_Status      setActionID (const MID&);

	Boolean		ping (int serverNum = BASE_SERVER);

//
//    	Additional operations provided by the RPC interface,
//	to allow the uses of atomic actions.
//

	ClientRajdoot_rpc (ClientRajdoot_rpc* doner);

protected:

    	RPC_Status 	softTerminate();

private:

//      Private Variables

	ServerStatus	status;

	int		numOfServers;
    	Int32         	retry, timeout;
        Int32            burstSize, burstTime;
    	OBJECT_DESC  	*objects;
	char		*defaultServiceName;

    	GROUPS       	*callGroup;
    	MID          	actionId;
    	FILTER_INFO  	callFilter;
    	FILTER_INFO  	actionGroupFilter;
};

#endif
