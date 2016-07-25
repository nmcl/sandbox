/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClANSA_.cc,v 1.4 1993/03/18 15:05:59 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClANSA_.cc,v 1.4 1993/03/18 15:05:59 nmcl Exp $";

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ANSA_CLANSA_H_
#  include <RPC/ANSA/ClANSA.h>
#endif

#ifndef ARJSERVERS_ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#define UNKNOWNSERVICE          "TheUnknownService"

extern "C"
{
#include "../../src/tRPC.h"
extern ansa_InterfaceRef nullRef;
extern rpc_initiate(OBJECT_DESC *, int *);
extern rpc_call(RPCBuff *, RPCBuff *, OBJECT_DESC *);
extern rpc_terminate(OBJECT_DESC *);
}

struct ObjectInfo
{
    OBJECT_DESC *desc;
};

ClientANSA_rpc::ClientANSA_rpc( ArjunaName* ArjNam )
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpc::ClientANSA_rpc(ArjunaName* ArjNam)\n" << flush;
#endif

    status = UNINITIATED;
    objects = new ObjectInfo;
    objects->desc = NULL;
    defaultServiceName = "";
    SetNumberOfServers();
    if (ArjNam != NULL)
    {
	SetHost(ArjNam->GetRefHostName());
	SetServiceName(ArjNam->GetRefServiceName());
    }
}
