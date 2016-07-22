/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc_Marshall.cc
 */

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif


#include "NameServ_stub.h"

RpcBuffer& operator>> (RpcBuffer& rpcbuff, ReplicaDescriptor*& tounpack)
{
    int number;
    ReplicaDescriptor* ptr;

    if (!tounpack)
	tounpack = new ReplicaDescriptor(0);

    ptr = tounpack;

    rpcbuff >> number;

    for (int j = 0; j < number; j++)
    {
	int sz = 0;
	Uid u_id;
	Boolean tempVal;

	rpcbuff >> sz;

	char* Name = new char[sz+1];
	::memset(Name, '\0', sz+1);
	for (int i_0_ = 0; i_0_ < sz; i_0_++ )
	    rpcbuff >> Name[i_0_];
	ptr->SetHost(Name);
	delete Name;

	rpcbuff >> u_id;
	ptr->SetUid(u_id);
	
	rpcbuff >> tempVal;
	ptr->SetActive(tempVal);

	if (j + 1 < number)
	{
	    if (ptr->next)
		delete ptr->next;
	    
	    ptr->next = new ReplicaDescriptor(0);
	    ptr = ptr->next;
	}
	else
	    ptr->next = 0;
    }

    return rpcbuff;
}

RpcBuffer& operator>> (RpcBuffer& rpcbuff, ReplicaDescriptor& tounpack)
{
    int number;
    ReplicaDescriptor* ptr = &tounpack;

    rpcbuff >> number;

    for (int j = 0; j < number; j++)
    {
	int sz = 0;
	Uid u_id;
	Boolean tempVal;
	
	rpcbuff >> sz;
	char* Name = new char[sz+1];
	::memset(Name, '\0', sz+1);
	for (int i_0_ = 0; i_0_ < sz; i_0_++ )
	    rpcbuff >> Name[i_0_];
	ptr->SetHost(Name);

	rpcbuff >> u_id;
	ptr->SetUid(u_id);

	rpcbuff >> tempVal;
	ptr->SetActive(tempVal);

	if (j + 1 < number)
	{
	    if (ptr->next)
		delete ptr->next;
	    
	    ptr->next = new ReplicaDescriptor(0);
	    ptr = ptr->next;
	}
	else
	    ptr->next = 0;
    }

    return rpcbuff;
}

RpcBuffer& operator<< (RpcBuffer& rpcbuff, ReplicaDescriptor* topack)
{
    int number = 0;
    ReplicaDescriptor* ptr = topack;

    if (!topack)
	return rpcbuff;

    while (ptr != 0)
    {
	number++;
	ptr = ptr->next;
    }

    ptr = topack;
    rpcbuff << number;
    for (int j = 0; j < number; j++)
    {
	int sz = strlen(ptr->GetHost());
	char* Name = new char[sz +1];
	::memset(Name, '\0', sz+1);
	::memcpy(Name, ptr->GetHost(), sz);
	
	rpcbuff << sz;
	for (int i_1_ = 0; i_1_ < sz; i_1_++ )
	    rpcbuff << Name[i_1_];
	delete Name;
	
	rpcbuff << *ptr->GetUid();
	rpcbuff << ptr->GetActive();
	ptr = ptr->next;
    }

    return rpcbuff;
}

RpcBuffer& operator<< (RpcBuffer& rpcbuff, const ReplicaDescriptor* topack)
{
    int number = 0;
    const ReplicaDescriptor* ptr = topack;

    if (!topack)
	return rpcbuff;

    while (ptr != 0)
    {
	number++;
	ptr = ptr->next;
    }

    ptr = (ReplicaDescriptor*) topack;
    rpcbuff << number;
    for (int j = 0; j < number; j++)
    {
	int sz = strlen(ptr->GetHost());
	char* Name = new char[sz+1];
	::memset(Name, '\0', sz+1);
	::memcpy(Name, ptr->GetHost(), sz);
	rpcbuff << sz;
	for (int i_1_ = 0; i_1_ < sz; i_1_++ )
	    rpcbuff << Name[i_1_];
	delete Name;
	
	rpcbuff << *ptr->GetUid();
	rpcbuff << ptr->GetActive();
	ptr = ptr->next;
    }

    return rpcbuff;
}

RpcBuffer& operator<< (RpcBuffer& rpcbuff, ReplicaDescriptor& topack)
{
    int number = 0;
    ReplicaDescriptor* ptr = &topack;

    while (ptr != 0)
    {
	number++;
	ptr = ptr->next;
    }

    ptr = &topack;
    rpcbuff << number;
    for (int j = 0; j < number; j++)
    {
	int sz = strlen(ptr->GetHost());
	char* Name = new char[sz+1];
	::memset(Name, '\0', sz+1);
	::memcpy(Name, ptr->GetHost(), sz);
	
	rpcbuff << sz;
	for (int i_1_ = 0; i_1_ < sz; i_1_++ )
	    rpcbuff << Name[i_1_];
	delete Name;

	rpcbuff << *ptr->GetUid();
	rpcbuff << ptr->GetActive();
	ptr = ptr->next;
    }

    return rpcbuff;
}
