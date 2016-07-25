/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClANSAAct.h,v 1.2 1993/03/18 14:23:58 nmcl Exp $
 */

#ifndef ANSA_CLRAJACT_H_
#define ANSA_CLRAJACT_H_

/*
 * This class provides C++ interface to the client side of the
 * ANSA rpc protocol, which is tailored for "atomic actions".
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ANSA_CLRAJ_H_
#  include <RPC/ANSA/ClANSA.h>
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
class ClientANSAAction_rpc : public ClientANSA_rpc
{
public:

//
//    	Fundermental generic operations provided by
//	the RPC interface.
//

    	ClientANSAAction_rpc(ArjunaName* ArjName);
    	ClientANSAAction_rpc(const char* serviceName);
    	virtual ~ClientANSAAction_rpc();

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

	ClientANSAAction_rpc(ClientANSAAction_rpc* doner);
private:
	void		PackActionHierarchy(Buffer& hierBuff);

	Boolean	        is_zombie;
	Uid             obj_uid;
};

#endif
