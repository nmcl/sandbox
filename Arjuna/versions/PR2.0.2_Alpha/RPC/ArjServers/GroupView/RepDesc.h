/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc.h,v
 */

#ifndef REPLICADESC_H_
#define REPLICADESC_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef REPLICADATA_H_
#  include "ReplicaData.h"
#endif


// @NoRemote, @NoMarshall
class RpcBuffer;

// @NoRemote, @NoMarshall
class ostream;


//@NoRemote, @NoMarshall
class GroupData;

// @NoRemote, @UserMarshall
class ReplicaDescriptor : public ReplicaData
{
    friend RpcBuffer& operator<< (RpcBuffer&, ReplicaDescriptor*);
    friend RpcBuffer& operator<< (RpcBuffer&, ReplicaDescriptor&);
    friend RpcBuffer& operator<< (RpcBuffer&, const ReplicaDescriptor*);

public:
    ReplicaDescriptor (const ReplicaDescriptor&);
    ReplicaDescriptor ();
    virtual ~ReplicaDescriptor ();

    Boolean pack (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    // the important operations!

    ReplicaDescriptor& operator= (const ReplicaDescriptor&);
    ReplicaDescriptor& operator= (const GroupData&);
    ReplicaDescriptor& operator+= (const GroupData&);
    ReplicaDescriptor& operator+= (const ReplicaDescriptor&);
    Boolean operator== (const ReplicaDescriptor&);
    Boolean operator!= (const ReplicaDescriptor&);

    void getGroupList (const Uid&, ReplicaDescriptor*&, Boolean = FALSE) const;

    virtual ostream& print (ostream&) const;

private:
    ReplicaDescriptor* getTemplate (const Uid&, Boolean) const;
    void getReplicaView (const Uid&, ReplicaDescriptor*&) const;
    ReplicaDescriptor* getOwnTemplate (const Uid&, Boolean) const;
    void makeViewFromTemplate (const Uid&, ReplicaDescriptor*&, Boolean = FALSE) const;

    long getNumber () const;

public:
    void getCachedGroupList (const Uid&, ReplicaDescriptor*&) const;
    void testAndSetObjectNames (const TypeName);

    void marshall (RpcBuffer&) const;
    void unmarshall (RpcBuffer&);

    ReplicaDescriptor* next;
};

extern ostream& operator<< (ostream& strm, const ReplicaDescriptor& rd);

extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ReplicaDescriptor* topack);
extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, ReplicaDescriptor& topack);
extern RpcBuffer& operator<< ( RpcBuffer& rpcbuff, const ReplicaDescriptor* topack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ReplicaDescriptor*& tounpack);
extern RpcBuffer& operator>> ( RpcBuffer& rpcbuff, ReplicaDescriptor& tounpack);

extern const ReplicaDescriptor NIL_RD;

#endif
