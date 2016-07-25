/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RPObjStoreIF.h,v
 */

#ifndef RPOBJSTOREIF_H_
#define RPOBJSTOREIF_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif


class RemotePersistentObjectStore;
class ReplicaDescriptor;


class RPObjStoreList
{
    friend class RPObjStoreInterface;
    friend RemotePersistentObjectStore;

public:
    RemotePersistentObjectStore* getStore (const Uid&, const TypeName);
    void removeStore (RemotePersistentObjectStore*&);

    ReplicaDescriptor* getCachedView (const Uid&, RPObjStoreList*&) const;
    Boolean reSyncCachedView (const Uid&, const TypeName, RemotePersistentObjectStore*);

private:
    RPObjStoreList ();
    ~RPObjStoreList ();

    RemotePersistentObjectStore* instance;
    RPObjStoreList* derivedFrom;
    unsigned long useCount;
    RPObjStoreList *next, *prev;
};


class RPObjStoreInterface : public ObjectStore
{
    friend RPObjStoreList;
    friend RemotePersistentObjectStore;
    
public:
    RPObjStoreInterface ();
    virtual ~RPObjStoreInterface ();

    Boolean commit_state (const Uid&, const TypeName);
    ObjectState *read_state (const Uid&, const TypeName);
    ObjectState *read_uncommitted (const Uid&, const TypeName);
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);
    ObjectState *allObjUids (const TypeName);

    const char* getStoreName () const;
    const TypeName type () const;
    void storeLocation (const char*);
    void useStoreLocation (const Boolean);

private:
    void setupStore (const Uid&, const TypeName);

    RemotePersistentObjectStore* instanceRPOS;
    Boolean delayedAction, useStore;
    const char* locationToSet;

    static RPObjStoreList *headOfList, *tailOfList;
};


#endif

