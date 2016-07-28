#include "LockMan_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, LockResult* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, LockResult& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (LockResult)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, LockResult*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

