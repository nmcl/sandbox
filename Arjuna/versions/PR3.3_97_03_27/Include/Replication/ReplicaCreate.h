/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaCreate.h,v 1.13 1994/08/30 08:09:08 ngdp Exp $
 */

#ifndef REPLICACREATE_H_
#define REPLICACREATE_H_

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


// @NoRemote, @UserMarshall
class ReplicaDescriptor;

// @NoRemote, @NoMarshall
class GroupData;

// @NoRemote, @NoMarshall
class RemoteRPObjStore;

// @NoRemote, @NoMarshall
class NSInterface;

class ClientRpc;
class ClientRpcManager;


//@NoRemote, @NoMarshall
enum StoreState { UNKNOWN, NOTPRESENT, SIZEERROR, OK };


//@NoRemote, @NoMarshall
class StateData
{
public:
    StateData ();
    ~StateData ();

    ClientRpc*         RpcClient;
    ClientRpcManager*  RpcMan;
    RemoteRPObjStore*  SD;
    char*              hostName;

    StateData*         next;
};


//@NoRemote, @NoMarshall
class ReplicaCreate
{
    friend class RegisterGroup;

public:
    ReplicaCreate (const Uid&, TypeName);
    ReplicaCreate ();
    virtual ~ReplicaCreate ();

    /* Methods concerned with creation of new replica group. */

    void setObjectInfo (const Uid&, TypeName, const char* = 0);
    void setState (ObjectState*);

    /* Modification of group data *prior* to its creation. */

    Boolean addReplica (const char*, const Uid& = NIL_UID, Boolean = FALSE);
    Boolean deleteReplica (const char*, const Uid& = NIL_UID);

    /* Create *new* replica group. */

    StoreState createReplicas (unsigned long = 1);
    StoreState createReplicas (const ReplicaDescriptor&, unsigned long = 1);

    /* Methods concerned with modification of existing replica groups. */

    Boolean addToGroup      (const char*, const Uid& = NIL_UID, Boolean = FALSE);
    Boolean addToGroup      (ReplicaDescriptor*&, const char*, const Uid& = NIL_UID, Boolean = FALSE);

    Boolean removeFromGroup (const char*, const Uid& = NIL_UID);
    Boolean removeFromGroup (ReplicaDescriptor*&, const char*, const Uid& = NIL_UID);

    Boolean copyGroup       (const ReplicaDescriptor&, const GroupData&);

    const ReplicaDescriptor& getView () const;

    /* Reset all internal data ready for re-use of object.*/
    
    void reset ();

protected:
    Boolean addToGroup      (const char*, const Uid&, LockMode, Boolean = FALSE);
    Boolean removeFromGroup (const char*, const Uid&, LockMode);
    
    Boolean needToRelease () const;
    Boolean releaseGroup ();

private:
    StoreState readObjectState (ReplicaDescriptor*, ObjectState*&);
    StoreState writeObjectState (ReplicaDescriptor*, const ObjectState&);

    Boolean getReplicaInformation (LockMode);

    RemoteRPObjStore* createDaemon (const char*);
    Boolean activateDatabase ();

    Uid                *register_uid;
    TypeName           tn;
    StateData          *head;
    ReplicaDescriptor  *rd, *endOfList;
    NSInterface        *GV;
    Boolean            doRelease, copyFrom;
    unsigned long      groupSize;
    ObjectState        *os;
    const char         *objectStoreRoot;
};


#include <Replication/ReplicaCreate.n>

#endif
