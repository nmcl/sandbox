#include "StateMan_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ObjectType* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ObjectType& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (ObjectType)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ObjectType*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

