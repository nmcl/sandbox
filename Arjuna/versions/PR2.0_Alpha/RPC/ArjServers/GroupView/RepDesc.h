/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc.h
 */

#ifndef REPLICADESC_H_
#define REPLICADESC_H_

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

// @NoRemote, @NoMarshall
class RpcBuffer;

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @NoMarshall
class GroupData;

// @NoRemote, @UserMarshall
class ReplicaDescriptor
{
public:
    ReplicaDescriptor (char*);
    ReplicaDescriptor ();
    virtual ~ReplicaDescriptor ();

    void SetHost (char*);
    void SetUid (const Uid&);
    void SetActive (Boolean);
    
    char* GetHost () const;
    Uid* GetUid () const;
    Boolean GetActive () const;

    long GetNumber () const;
    
    ReplicaDescriptor& operator= (const ReplicaDescriptor&);
    ReplicaDescriptor& operator= (const GroupData&);
    
    Boolean operator== (const ReplicaDescriptor&);

    virtual ostream& print (ostream&) const;

    ReplicaDescriptor* next;

private:
    char* destn_host;
    Uid* u;
    Boolean active;
};

extern ostream& operator<< (ostream& strm, const ReplicaDescriptor& rd);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ReplicaDescriptor* topack);

extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ReplicaDescriptor*& tounpack);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ReplicaDescriptor& topack);

extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ReplicaDescriptor& tounpack);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const ReplicaDescriptor* topack);

#endif
