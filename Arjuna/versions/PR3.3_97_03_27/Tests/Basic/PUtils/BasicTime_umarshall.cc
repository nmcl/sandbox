/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: BasicTime_umarshall.cc,v 1.1 1994/01/06 12:21:01 ngdp Exp $
 *
 */

#include "BasicTime_stub.h"

#include <StubGen/RpcBuffer.h>

void BasicTime::marshall ( RpcBuffer& rpcBuff ) const
{
#ifdef SYSV
        rpcBuff << theTime;
#else
        rpcBuff << theTime.tv_sec;
        rpcBuff << theTime.tv_usec;
#endif
}


void BasicTime::unmarshall ( RpcBuffer& rpcBuff )
{
#ifdef SYSV
        rpcBuff >> theTime;
#else
        rpcBuff >> theTime.tv_sec;
        rpcBuff >> theTime.tv_usec;
#endif
}
