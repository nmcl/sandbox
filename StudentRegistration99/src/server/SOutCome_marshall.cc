#include "SOutCome_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, SOutCome* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, SOutCome& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (SOutCome)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, SOutCome*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

