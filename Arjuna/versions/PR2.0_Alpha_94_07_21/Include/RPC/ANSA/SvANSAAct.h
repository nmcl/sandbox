/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvANSAAct.h,v 1.3 1993/05/05 13:21:14 n048z Exp $
 */

#ifndef ANSA_SVANSAACT_H_
#define ANSA_SVANSAACT_H_

/*
 * This class provides C++ interface to the server side of the
 * ANSA rpc protocol, which is tailored for "atomic actions".
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ANSA_SVANSA_H_
#  include <RPC/ANSA/SvANSA.h>
#endif

class Buffer;
class Uid;
class ActionHierarchy;

//  @NoRemote, @NoMarshall
class ServerANSAAction_rpc : public ServerANSA_rpc
{
public:
    ServerANSAAction_rpc();
    virtual ~ServerANSAAction_rpc();

    int Initialise(int argc, char *argv[]);

    void GetWork(long& opcode, Buffer& call);
    void GetWork(long& opcode, Buffer& call1, Buffer& call2);

    void SendResult(long errcode, Buffer& result);
    void SendResult(long errcode, Buffer& result1, Buffer& result2);

private:
    Boolean FixHierarchy(Buffer& hierBuff);

    void DoLocal_NestedAbort(Buffer& call);
    void DoLocal_NestedCommit(Buffer& call);
    void DoLocal_NestedPrepare(Buffer& call);

    void DoLocal_TopLevelAbort(Buffer& call);
    void DoLocal_TopLevelCommit(Buffer& call);
    void DoLocal_TopLevelPrepare(Buffer& call);
};
#endif
