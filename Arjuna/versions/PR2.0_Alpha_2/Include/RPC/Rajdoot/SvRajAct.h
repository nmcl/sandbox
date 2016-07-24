/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvRajAct.h,v 1.1 1993/11/03 14:31:02 nmcl Exp $
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
    ServerRajdootAction_rpc();
    virtual ~ServerRajdootAction_rpc();

    int Initialise(int argc, char *argv[]);

    void GetWork(long& opcode, Buffer& call);
    void GetWork(long& opcode, Buffer& call1, Buffer& call2);

    void SendResult(long errcode, Buffer& result);
    void SendResult(long errcode, Buffer& result1, Buffer& result2);

protected:
    void DoGetWork(long& opcode, Buffer& call);
    void DoGetWork(long& opcode, Buffer& call1, Buffer& call2);

private:
    Boolean FixHierarchy(Buffer& hierBuff);

    void DoLocal_NestedAbort(Buffer& call);
    void DoLocal_NestedCommit(Buffer& call);
    void DoLocal_NestedPrepare(Buffer& call);

    void DoLocal_TopLevelAbort(Buffer& call);
    void DoLocal_TopLevelCommit(Buffer& call);
    void DoLocal_TopLevelPrepare(Buffer& call);
};


inline void ServerRajdootAction_rpc::GetWork(long& opcode, Buffer& call)
{
    ONDEADLINE();
    ServerRajdootAction_rpc::DoGetWork(opcode, call);
}

inline void ServerRajdootAction_rpc::GetWork(long& opcode, Buffer& call1, Buffer& call2)
{
    ONDEADLINE();
    ServerRajdootAction_rpc::DoGetWork(opcode, call1, call2);
}

#endif
