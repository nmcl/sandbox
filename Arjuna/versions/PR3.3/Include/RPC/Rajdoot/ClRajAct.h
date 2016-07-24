/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClRajAct.h,v 1.6 1995/09/25 15:02:11 nmcl Exp $
 */

#ifndef RAJDOOT_CLRAJACT_H_
#define RAJDOOT_CLRAJACT_H_

/*
 * This class provides C++ interface to the client side of the
 * Rajdoot rpc protocol, which is tailored for "atomic actions".
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RAJDOOT_CLRAJ_H_
#  include <RPC/Rajdoot/ClRaj.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef PREPOUT_H_
#  include <Arjuna/PrepOut.h>
#endif

class ArjunaName;
class Buffer;

// @NoRemote, @NoMarshall
class ClientRajdootAction_rpc : public ClientRajdoot_rpc
{
public:

//
//    	Fundermental generic operations provided by
//	the RPC interface.
//

    ClientRajdootAction_rpc (ArjunaName*);
    ClientRajdootAction_rpc (const char* const serviceName);
    virtual ~ClientRajdootAction_rpc ();

    RPC_Status initiate();

    RPC_Status call (Int32  opcode,  Buffer& callBuff,
		     Int32& errcode, Buffer& result);

    virtual RPC_Status 	terminate ();
    
    Boolean doRemoteNestedAbort ();
    Boolean doRemoteNestedCommit ();
    PrepareOutcome doRemoteNestedPrepare ();

    Boolean doRemoteTopLevelAbort ();
    Boolean doRemoteTopLevelCommit ();
    PrepareOutcome doRemoteTopLevelPrepare ();

    void enableAtomic ();
    void disableAtomic ();

    ClientRajdootAction_rpc (ClientRajdootAction_rpc* doner);
    
private:
    void packActionHierarchy (Buffer& hierBuff);
    void packNullActionHierarchy (Buffer& hierBuff);

    Boolean atomicEnabled;
    Boolean isZombie;
    Uid objUid;
};

#endif
