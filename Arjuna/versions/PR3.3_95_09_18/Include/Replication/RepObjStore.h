/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepObjStore.h,v 1.18 1996/10/09 13:04:07 nmcl Exp $
 */

#ifndef REPOBJSTORE_H_
#define REPOBJSTORE_H_

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

class ReplicaData;
class ReplicaDescriptor;
class GroupData;

//@NoRemote, @NoMarshall
class NSInterface;

//@NoRemote, @NoMarshall
class Members;

//@NoRemote, @NoMarshall
class DaemonList;


#ifndef __linux__
const int RepDebugTimeout = 8000;
const int RepDebugRetry = 8;
#else
const int RepDebugTimeout = 10000;
const int RepDebugRetry = 10;
#endif
const int RepNormalTimeout = 8000;
const int RepNormalRetry = 4;


// @NoRemote, @NoMarshall
class client_data
{
public:
    client_data ();
    ~client_data ();
    
    Boolean           groupCreated;   // is group setup yet
    pid_t             pid;            // process id of user of db
    ReplicaDescriptor *group_view;    // various views
    ReplicaDescriptor *compositeView;
    Uid               *group_uid;     // replica group id
    Uid               *actionID;      // keep track of which action view was obtained in
};


// @NoRemote, @NoMarshall
class ViewData
{
public:
    ViewData ();
    ~ViewData ();

    GroupData* gd;
    Members*   daemonInfo;
    ViewData*  next;
    Boolean    isAlive;
};


// @NoRemote, @NoMarshall
class ReplicatedPersistentObjectStore : public ObjectStore
{
    friend class RepObjStoreInterface;
    friend class RepObjStoreList;
    friend class ReplicationRecord;

public:
    enum DaemonStatus { AVAILABLE, UNAVAILABLE, UNKNOWN, V_PARTITION };
    
    ReplicatedPersistentObjectStore (const char* = "");
    virtual ~ReplicatedPersistentObjectStore ();

    /* The real interface */

    virtual Boolean allObjUids (const TypeName, ObjectState&, ObjectStore::StateStatus);
    virtual Boolean allTypes (ObjectState&) const;
    virtual const char* getStoreName () const;

    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
     
    virtual Boolean commit_state (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);
    
    virtual ObjectState *read_committed (const Uid&, const TypeName);
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName);
    virtual Boolean remove_committed (const Uid&, const TypeName);
    virtual Boolean remove_uncommitted (const Uid&, const TypeName);
    virtual Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    virtual Boolean write_uncommitted (const Uid&, const TypeName, const ObjectState&);
    
    const TypeName type () const;

    void storeLocation (char**, int = 0);
    void useStoreLocation (const Boolean);

    Boolean packInto (Buffer&) const;
    Boolean unpackFrom (Buffer&);
    
protected:
    void setRootOfStore (const char*);
    virtual Boolean supressEntry (const char *) const;
    
private:
	
    /* operations concerned with creating replica group members. */

    Boolean createNewObject (const Uid&, const TypeName, Boolean);
    Boolean createGroupView (const Uid&, const TypeName, Boolean);
    
    DaemonStatus activateDaemon (ReplicaDescriptor*);
    DaemonStatus createDaemon   (ReplicaDescriptor*);
    
    Boolean tryOldDaemon  (Members*, ReplicaDescriptor*);
    Boolean activateGroup (Boolean&, Boolean);

    Boolean deadServers  (Boolean);
    void    makeInactive (ReplicaData*, Boolean);

    Boolean activateDatabase   ();
    Boolean deactivateDatabase ();

    ReplicaDescriptor* getCompositeView  (const Uid&) const;
    Boolean            getViewAndExclude (const Uid&, const TypeName, Boolean);

    void cleanUp ();
    void undo ();

    static Boolean rpc_handler (RPC_Status, Int32);
    Boolean (*old_handler) (RPC_Status, Int32);

    Boolean                  isRemoteOnly, storeValid;
    Boolean                  failureDetected, doRelease;
    client_data              myData;  // stores the view as returned by the GVD (or user supplied)
    ViewData*                replica_members; // stores the current view
    DaemonList*              _failedReplicas;
    char**                   StoreLocation;
    char*                    localStoreName;
    int                      numberOfStores;
    NSInterface*             NS;

    static DaemonList*       daemons;
    static int               use_count;
    static char*             hostName;
};


// @NoRemote, @NoMarshall
class RepObjStoreSetUp : public ObjectStoreSetUp
{
public:
    RepObjStoreSetUp ();
    ~RepObjStoreSetUp ();
};

static RepObjStoreSetUp RepSetUp;

#include <Replication/RepObjStore.n>

#endif
