#include "FacCode_stub.h"
#include <StubGen/RpcBuffer.h>

RpcBuffer& operator<< ( RpcBuffer& rpcbuff, FacilityCode* enumPtr )
{
	return rpcbuff.operator<< ((int*)enumPtr);
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, FacilityCode& enumRef )
{
	int dummy;
	rpcbuff >> dummy;
	enumRef = (FacilityCode)dummy;
	return rpcbuff;
}

RpcBuffer& operator>> ( RpcBuffer& rpcbuff, FacilityCode*& enumPtr)
{
	return rpcbuff.operator>> ((int*&)enumPtr);
}

