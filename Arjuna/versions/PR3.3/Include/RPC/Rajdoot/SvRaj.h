/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: SvRaj.h,v 1.4 1994/12/09 11:38:22 ngdp Exp $
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

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RAJDOOT_ONDEADLINE_H_
#  include <RPC/Rajdoot/ondeadline.h>
#endif

class Buffer;

//  @NoRemote, @NoMarshall
class ServerRajdoot_rpc
{
public:
    ServerRajdoot_rpc ();
    virtual ~ServerRajdoot_rpc ();

    int initialise (int argc, char *argv[]);

    void getWork (Int32& opcode, Buffer& call);
    void getWork (Int32& opcode, Buffer& call1, Buffer& call2);

    void sendResult (Int32 errcode, Buffer& result);
    void sendResult (Int32 errcode, Buffer& result1, Buffer& result2);

    static char* getServerName ();

protected:
    void doGetWork (Int32& opcode, Buffer& call);
    void doGetWork (Int32& opcode, Buffer& call1, Buffer& call2);
private:
    Boolean    init;

    GROUPS    *client_addr;
    DEADLINE  deadlines;
};

inline void ServerRajdoot_rpc::getWork ( Int32& opcode, Buffer& call )
{
    ONDEADLINE();
    ServerRajdoot_rpc::doGetWork(opcode, call);
}

inline void ServerRajdoot_rpc::getWork ( Int32& opcode, Buffer& call1,
					 Buffer& call2 )
{
    ONDEADLINE();
    ServerRajdoot_rpc::doGetWork(opcode, call1, call2);
}

#endif
