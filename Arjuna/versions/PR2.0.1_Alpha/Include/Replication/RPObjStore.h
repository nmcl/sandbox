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

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif


class ReplicaDescriptor;
class Members;


const long DebugTimeout = 5000;
const long DebugRetry = 5;
const long NormalTimeout = 3000;
const long NormalRetry = 3;



class client_data
{
public:
    client_data ();
    ~client_data ();
    
    Boolean groupCreated;
    pid_t pid;
    ReplicaDescriptor *group_view, *compositeView;
    Uid *group_uid;
};


class ViewData
{
public:
    ViewData ();
    ~ViewData ();

    class GroupData* gd;    
    Members* daemonInfo;
    ViewData* next;
};


class RemotePersistentObjectStore : public ObjectStore
{
    friend class RPObjStoreList;
    friend class RPObjStoreInterface;
    
public:
    RemotePersistentObjectStore ();
    virtual ~RemotePersistentObjectStore ();

    /* The real interface */

    Boolean commit_state (const Uid&, const TypeName);
    ObjectState* read_state (const Uid&, const TypeName);
    ObjectState* read_uncommitted (const Uid&, const TypeName);
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);

    ObjectState *allObjUids (const TypeName tName);

    const TypeName type () const;
    const char* getStoreName () const;

    void storeLocation (const char*);
    void useStoreLocation (const Boolean);

private:
    // operations concerned with creating replica group members

    Boolean createNewObject (const Uid&);
    Boolean createGroupView (const Uid&, const TypeName, Boolean);
    Boolean activateDaemon (ReplicaDescriptor*);
    Boolean createDaemon (ReplicaDescriptor*);
    Boolean tryOldDaemon (Members*, ReplicaDescriptor*);
    Boolean activateGroup (Boolean&, Boolean);

    Boolean deadServers (Boolean);
    void makeInactive (class ReplicaData*, Boolean);

    Boolean activateDatabase ();
    Boolean deactivateDatabase ();

    ReplicaDescriptor* getCompositeView (const Uid&) const;
    Boolean getViewAndExclude (const Uid&, const TypeName, Boolean);
    
    void undo ();

    static Boolean rpc_handler ();
    Boolean (*old_handler) ();

    Boolean isRemoteOnly, storeValid, failureDetected;
    client_data myData;
    ViewData* replica_members;
    char *StoreLocation, *storeName;

    class NSInterface* NS;

    static class DaemonList* daemons;
    static int use_count;
    static char* hostName;
};


// @NoRemote, @NoMarshall
class RPObjStoreSetUp
{
public:
    RPObjStoreSetUp ();
    ~RPObjStoreSetUp ();

    static ObjectStore* create (const char*);
    static void destroy (const char*);

private:
    static Boolean setUp;
};

static RPObjStoreSetUp RPSetUp;

#include "RPObjStore.n"

#endif
