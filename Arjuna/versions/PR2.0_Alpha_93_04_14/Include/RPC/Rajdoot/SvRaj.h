/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SvRaj.h,v 1.10 1993/03/18 14:33:39 nmcl Exp $
 */

#ifndef RAJDOOT_SVRAJ_H_
#define RAJDOOT_SVRAJ_H_

/*
 * This class provides C++ interface to the server side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

class Buffer;

//  @NoRemote, @NoMarshall
class ServerRajdoot_rpc
{
public:
    ServerRajdoot_rpc();
    virtual ~ServerRajdoot_rpc();

    int Initialise(int argc, char *argv[]);

    void GetWork(long& opcode, Buffer& call);
    void GetWork(long& opcode, Buffer& call1, Buffer& call2);

    void SendResult(long errcode, Buffer& result);
    void SendResult(long errcode, Buffer& result1, Buffer& result2);

    static char* ServerName;

protected:
    void DoGetWork(long& opcode, Buffer& call);
    void DoGetWork(long& opcode, Buffer& call1, Buffer& call2);
private:
    Boolean    init;

    GROUPS    *client_addr;
    DEADLINE  deadlines;
};

inline void ServerRajdoot_rpc::GetWork(long& opcode, Buffer& call)
{
    ONDEADLINE();
    ServerRajdoot_rpc::DoGetWork(opcode, call);
}

inline void ServerRajdoot_rpc::GetWork(long& opcode, Buffer& call1, Buffer& call2)
{
    ONDEADLINE();
    ServerRajdoot_rpc::DoGetWork(opcode, call1, call2);
}

#endif
