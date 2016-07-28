#include "StateMan_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ObjectStatus* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ObjectStatus& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (ObjectStatus)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ObjectStatus*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

