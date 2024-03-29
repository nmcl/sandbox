/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaData.h,v
 */

#ifndef REPLICADATA_H_
#define REPLICADATA_H_


#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif


// @NoRemote, @NoMarshall
class RpcBuffer;

// @NoRemote, @NoMarshall
class ostream;

extern const char* nullRDName;
extern const char* nullObjectName;


//@NoRemote, @UserMarshall
class ReplicaData
{
    friend RpcBuffer& operator<< (RpcBuffer&, const ReplicaData*);
    friend RpcBuffer& operator>> (RpcBuffer&, ReplicaData*&);

public:
    virtual ~ReplicaData ();

    void setHost (char*);
    void setObjectName (char*);
    void setUid (const Uid&);
    void setGroupUid (const Uid&);
    void setActive (Boolean);

    const char* getHost () const;
    const char* getObjectName () const;
    const Uid& getUid () const;
    const Uid& getGroupUid () const;
    Boolean getActive () const;

    Boolean isNullType () const;
    Boolean isTemplate () const;
    Boolean isWildCard () const;
    Boolean isGlobalWildCard () const;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    virtual ostream& print (ostream&) const;

    virtual void marshall (RpcBuffer&) const;
    virtual void unmarshall (RpcBuffer&);

protected:
    ReplicaData ();
    
private:
    void Initialize ();

    char* destn_host;
    char* objectName;
    Uid* u;
    Uid* groupUid;
    Boolean active;
};

extern ostream& operator<< (ostream& strm, const ReplicaData& rd);

#include <RPC/ArjServers/ReplicaData.n>

#endif

