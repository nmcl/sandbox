/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RPObjStore.h,v
 */

#ifndef RPOBJSTORE_H_
#define RPOBJSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Object Store Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef OBJSTORE_H_
#include "ObjStore.h"
#endif

class ostream;

class Buffer;
class ObjectState;
class Uid;

// @NoRemote, @NoMarshall
class ClientRpc;

// @NoRemote, @NoMarshall
class RpcControl;

// @NoRemote, @NoMarshall
class NSInterface;

// @NoRemote, @NoMarshall
class RemoteStateDaemon;

// @NoRemote, @UserMarshall
class ReplicaDescriptor;

// @NoRemote, @NoMarshall
class Members
{
public:
    Members ();
    ~Members ();

    ClientRpc* CRPC;
    RpcControl* RPCCont;
    RemoteStateDaemon* r_state_daemon;
    ReplicaDescriptor* rd;
    
    Members* next;
};

// @NoRemote, @NoMarshall
class DaemonData
{
public:
    DaemonData ();
    ~DaemonData ();

    Members* memb;
    char* hostName;
    Boolean available;
};

// @NoRemote, @NoMarshall
class DaemonList
{
public:
    DaemonList ();
    ~DaemonList ();

    RemoteStateDaemon* Get (const char*);
    void Put (Members*, const char*);
    void Delete (const char*);

private:
    DaemonList* next;
    DaemonData* data;
};

// @NoRemote, @NoMarshall
class RemotePersistentObjectStore : public ObjectStore
{
public:
    RemotePersistentObjectStore ();
    virtual ~RemotePersistentObjectStore ();

    /* The real interface */

    ObjectState *allObjUids (const TypeName tName);

    Boolean commit_state (const Uid&, const TypeName);
    ObjectState* read_state (const Uid&, const TypeName);
    ObjectState* read_uncommitted (const Uid&, const TypeName);
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);

    const TypeName type () const;
    void storeLocation (const char*);

private:
    const char* getStoreName () const;
    Boolean createNewObject (const Uid);
    Boolean create_groupview (const Uid, const TypeName);

    Boolean activateDaemon (ReplicaDescriptor*, Members*);
    Boolean activate_primary ();
    Boolean activate_backups ();

    /* If action must abort then undo operations. */
    void undo ();

    static Boolean rpc_handler ();
    Boolean (*old_handler) ();

    const char* storeName;
    char* StoreLocation;
    Boolean storeValid;

    static DaemonList* daemons;
    static int use_count;
    Members* replica_members; // Maintains information on replica group members
    NSInterface* NS; // Interface to replicated name server
    ReplicaDescriptor* group_view; // GroupView of replica group being used

    Uid* primary_uid; // Uid of primary replica
    Uid* group_uid; // Group Uid

    /* Used to indicate how far replication protocol has gone. */
    Boolean created_primary, created_backups;
    Boolean replica_failure, group_failure;
};

#endif
