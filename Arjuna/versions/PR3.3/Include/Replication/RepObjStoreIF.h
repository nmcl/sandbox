/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepObjStoreIF.h,v 1.5 1995/02/23 15:06:59 nmcl Exp $
 */

#ifndef REPOBJSTOREIF_H_
#define REPOBJSTOREIF_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef REPOBJSTORE_H_
#  include <Replication/RepObjStore.h>
#endif


//@NoRemote, @UserMarshall
class ReplicaDescriptor;


//@NoRemote, @NoMarshall
class RepObjStoreList
{
    friend class RepObjStoreInterface;
    friend ReplicatedPersistentObjectStore;
    friend class GroupInformation;

public:
    ReplicatedPersistentObjectStore* getStore (const Uid&, const TypeName);
    Boolean removeStore (ReplicatedPersistentObjectStore*&, Boolean = TRUE);

    ReplicaDescriptor* getCachedView (const Uid&, RepObjStoreList*&) const;
    Boolean reSyncCachedView (const Uid&, const TypeName, ReplicatedPersistentObjectStore*);

protected:
    RepObjStoreList ();
    virtual ~RepObjStoreList ();
    
private:
    void checkDerivedFrom (RepObjStoreList*, ReplicatedPersistentObjectStore*);

    ReplicatedPersistentObjectStore *instance;
    RepObjStoreList                 *derivedFrom;
    RepObjStoreList                 *next, *prev;
    unsigned long                   useCount;
};


//@NoRemote, @NoMarshall
class RepObjStoreInterface : public ObjectStore
{
    friend RepObjStoreList;
    friend ReplicatedPersistentObjectStore;
    friend class RepObjStoreSetUp;
    friend class ReplicationRecord;
    friend class GroupInformation;
    
public:
    RepObjStoreInterface (const char* = "", Boolean = FALSE);
    virtual ~RepObjStoreInterface ();

    virtual Boolean allObjUids (const TypeName, ObjectState&, ObjectStore::StateStatus);
    virtual Boolean allTypes (ObjectState&) const;

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
    virtual const char *getStoreName () const;
    
    void storeLocation (char**, int);
    void useStoreLocation (const Boolean);
    void setObjectData (const Uid&, const TypeName);

    Boolean packInto (Buffer&) const;
    Boolean unpackFrom (Buffer&);

    Boolean deactivate (Boolean = FALSE);
    
protected:
    virtual Boolean supressEntry (const char *) const;
 
private:
    void setupStore (const Uid&, const TypeName);
    void cleanUp ();

    ReplicatedPersistentObjectStore  *instanceRPOS;     // this is a reference to the real object
    char                             **locationToSet;
    int                              numberOfLocations;
    Boolean                          delayedAction, useStore, holdCache;
    TypeName                         tName;
    Uid                              gUID;
    char                             *localStoreName;
    
    static RepObjStoreList           *headOfList, *tailOfList;
};


#include <Replication/RepObjStoreIF.n>

#endif

