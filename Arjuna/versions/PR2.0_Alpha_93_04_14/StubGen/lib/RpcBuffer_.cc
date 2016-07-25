/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcBuffer_.cc,v 1.2 1993/05/06 14:49:46 nsmw Exp $
 */

static const char RCSid[] = "$Id: RpcBuffer_.cc,v 1.2 1993/05/06 14:49:46 nsmw Exp $";

/*
 * Class to handle RPC system buffers. This version inherits from Arjuna 
 * Buffers directly
 *
 */

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

/* Externally callable operators for some Arjuna classes */

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ObjectState *& buff )
{
    void *p;
    long index;
    
    if (rpcbuff.unpackPtr(p,index))
    {
	if (buff == 0)
	    buff = new ObjectState();
	buff->unpackFrom(rpcbuff);
    }
    else
        buff = (ObjectState *)rpcbuff.pointerAt(index);
    
    return rpcbuff;
}
