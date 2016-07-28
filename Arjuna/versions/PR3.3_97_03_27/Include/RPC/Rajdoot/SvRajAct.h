/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvRajAct.h,v 1.3 1994/12/09 11:38:23 ngdp Exp $
 */

#ifndef RAJDOOT_SVRAJACT_H_
#define RAJDOOT_SVRAJACT_H_

/*
 * This class provides C++ interface to the server side of the
 * Rajdoot rpc protocol, which is tailored for "atomic actions".
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RAJDOOT_SVRAJ_H_
#  include <RPC/Rajdoot/SvRaj.h>
#endif

class Buffer;
class Uid;
class ActionHierarchy;

//  @NoRemote, @NoMarshall
class ServerRajdootAction_rpc : public ServerRajdoot_rpc
{
public:
    ServerRajdootAction_rpc ();
    virtual ~ServerRajdootAction_rpc ();

    int initialise (int argc, char *argv[]);

    void getWork (Int32& opcode, Buffer& call);
    void getWork (Int32& opcode, Buffer& call1, Buffer& call2);

    void sendResult (Int32 errcode, Buffer& result);
    void sendResult (Int32 errcode, Buffer& result1, Buffer& result2);

protected:
    void doGetWork (Int32& opcode, Buffer& call);
    void doGetWork (Int32& opcode, Buffer& call1, Buffer& call2);

private:
    Boolean fixHierarchy (Buffer& hierBuff);

    void doLocalNestedAbort (Buffer& call);
    void doLocalNestedCommit (Buffer& call);
    void doLocalNestedPrepare (Buffer& call);

    void doLocalTopLevelAbort (Buffer& call);
    void doLocalTopLevelCommit (Buffer& call);
    void doLocalTopLevelPrepare (Buffer& call);
};


inline void ServerRajdootAction_rpc::getWork (Int32& opcode, Buffer& call )
{
    ONDEADLINE();
    ServerRajdootAction_rpc::doGetWork(opcode, call);
}

inline void ServerRajdootAction_rpc::getWork ( Int32& opcode, Buffer& call1,
					       Buffer& call2 )
{
    ONDEADLINE();
    ServerRajdootAction_rpc::doGetWork(opcode, call1, call2);
}

#endif
