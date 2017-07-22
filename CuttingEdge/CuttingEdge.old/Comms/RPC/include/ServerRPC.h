/*
 * Copyright 1990, 1991,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

#ifndef SERVERRPC_H
#define SERVERRPC_H

/*
 * $Id: ServerRPC.h,v 1.2 1992/01/28 11:47:57 nmcl Exp $
 */

#include <RPC.h>
#include <PortAddress.h>
#include <ServerManager.h>

class ServerRPC
{
private:
    ServerManager* Manager;
    PortAddress* ServerAddr;
    PortAddress* ClientAddr;
public:
    ServerRPC();
    ~ServerRPC();

    boolean Initialise(int argc, char *argv[]);

    void GetWork(long& opcode, Buffer& call);
    void GetWork(long& opcode, Buffer& call1, Buffer& call2);

    void SendResult(long errcode, Buffer& result);
    void SendResult(long errcode, Buffer& result1, Buffer& result2);
private:
};

#endif SERVERRPC_H
