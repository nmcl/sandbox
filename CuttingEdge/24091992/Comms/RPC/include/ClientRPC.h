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

#ifndef CLIENTRPC_H
#define CLIENTRPC_H

/*
 * $Id: ClientRPC.h,v 1.2 1992/01/28 11:47:53 nmcl Exp $
 */

#include <Port.h>
#include <PortAddress.h>
#include <RPC.h>
#include <ClientManager.h>

class ClientRPC
{
private:
    ClientManager* Manager;
    char* service;
    PortAddress *ClientAddr, *ServerAddr;
public:
    ClientRPC(const char* serviceName = NULL);
    ~ClientRPC();

    RPC_Status Initiate();
    RPC_Status Call(long  opcode,  Buffer& call,
		    long& errcode, Buffer& result);
    RPC_Status Terminate();
};

#endif CLIENTRPC_H
