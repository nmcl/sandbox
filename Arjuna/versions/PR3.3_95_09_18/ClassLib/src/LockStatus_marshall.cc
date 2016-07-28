#include "Lock_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, LockStatus* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, LockStatus& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (LockStatus)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, LockStatus*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

