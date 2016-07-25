/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef ANSA_SVANSA_H_
#define ANSA_SVANSA_H_

/*
 * $Id: SvANSA.h,v 1.4 1993/05/05 13:21:10 n048z Exp $
 *
 * This class provides C++ interface to the server side of the
 * ANSA rpc protocol.
 *
 */

#ifndef BUFFER_H_
#   include <Common/Buffer.h>
#endif

class ServerANSA_rpc
{
public:
    ServerANSA_rpc();
    virtual ~ServerANSA_rpc();

    int Initialise(int argc, char *argv[]);

    void GetWork(long& opcode, Buffer& call);
    void GetWork(long& opcode, Buffer& call1, Buffer& call2);

    void SendResult(long errcode, Buffer& result);
    void SendResult(long errcode, Buffer& result1, Buffer& result2);

    static char* ServerName;
};

#endif
