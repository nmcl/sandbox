#include "Error_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ErrorSeverity* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ErrorSeverity& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (ErrorSeverity)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ErrorSeverity*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

