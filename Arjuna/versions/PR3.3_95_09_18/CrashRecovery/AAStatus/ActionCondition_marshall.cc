#include "AAStatus_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ActionCondition* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ActionCondition& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (ActionCondition)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ActionCondition*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

