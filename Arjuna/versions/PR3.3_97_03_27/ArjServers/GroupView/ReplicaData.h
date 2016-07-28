/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaData.h,v 1.8 1994/10/04 10:22:14 nmcl Exp $
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

    void setHost (const char*);
    void setObjectName (const char*);
    void setStoreRoot (const char*);
    void setUid (const Uid&);
    void setGroupUid (const Uid&);
    void setActive (Boolean);

    const char* getHost () const;
    const char* getObjectName () const;
    const char* getStoreRoot () const;
    const Uid& getUid () const;
    const Uid& getGroupUid () const;
    Boolean getActive () const;

    Boolean isNullHost () const;
    Boolean isNullType () const;
    Boolean isTemplate () const;
    Boolean isWildCard () const;
    Boolean isGlobalWildCard () const;

    Boolean pack (Buffer&, int = -1) const;
    Boolean unpack (Buffer&, int = -1);

    virtual ostream& print (ostream&) const;

    virtual void marshall (RpcBuffer&) const;
    virtual void unmarshall (RpcBuffer&);

protected:
    ReplicaData ();
    
private:
    void Initialize ();

    char* destn_host;
    char* objectName;
    char* rootOfStore;
    Uid* _uid;
    Uid* _groupUid;
    Boolean active;
};

#include <ArjServers/ReplicaData.n>

#endif

