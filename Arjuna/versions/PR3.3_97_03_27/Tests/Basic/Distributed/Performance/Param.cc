/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: Param.cc,v 1.2 1994/09/02 10:28:00 ngdp Exp $
 */

#include <System/memory.h>
#include <StubGen/RpcBuffer.h>

#include "Param_stub.h"

Param::Param ( ) 
             : p(0),
	       size(0)
{
}

Param::Param ( size_t sz )
             : size(sz)
{
    p = new char[size];
    ::memset(p, 'a', size);
}

Param::~Param ()
{
    if (p)
	delete p;
    size = 0;
}

void Param::marshall ( RpcBuffer& buff ) const
{
    buff.packMappedBlock(p, size);
}

void Param::unmarshall ( RpcBuffer& buff )
{
    if (p)
	delete p;
    p = 0;
    
    buff.unpack(p, size);
}
