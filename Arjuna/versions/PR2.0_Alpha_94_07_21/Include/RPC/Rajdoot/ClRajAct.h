/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClRajAct.h,v 1.10 1993/03/18 14:33:35 nmcl Exp $
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

    	ClientRajdootAction_rpc(ArjunaName* ArjName);
    	ClientRajdootAction_rpc(const char* serviceName);
    	virtual ~ClientRajdootAction_rpc();

    	RPC_Status 		Initiate();

    	RPC_Status 		Call(long  opcode,  Buffer& call,
				     long& errcode, Buffer& result);

    	virtual	RPC_Status 	Terminate();

	void            DoRemote_NestedAbort();
	void            DoRemote_NestedCommit();
	PrepareOutcome  DoRemote_NestedPrepare();

	void            DoRemote_TopLevelAbort();
	void            DoRemote_TopLevelCommit();
	PrepareOutcome  DoRemote_TopLevelPrepare();

	ClientRajdootAction_rpc(ClientRajdootAction_rpc* doner);
private:
	void		PackActionHierarchy(Buffer& hierBuff);

	Boolean	        is_zombie;
	Uid             obj_uid;
};

#endif
