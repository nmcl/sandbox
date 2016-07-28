#include "LockMan_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ConflictType* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ConflictType& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (ConflictType)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ConflictType*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

