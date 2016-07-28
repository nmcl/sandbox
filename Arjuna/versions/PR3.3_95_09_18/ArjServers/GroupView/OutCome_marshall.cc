#include "NameServ_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, OutCome* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, OutCome& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (OutCome)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, OutCome*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

