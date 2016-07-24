/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LPObjStore.h,v 1.1 1993/11/03 14:29:02 nmcl Exp $
 */

#ifndef LPOBJSTORE_H_
#define LPOBJSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Local Object Store Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef STORELIST_H_
#  include <ObjectStore/StoreList.h>
#endif

#ifndef FDCACHE_H_
#  include <ObjectStore/FdCache.h>
#endif

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class ObjectState;

// @NoRemote, @NoMarshall
class LocalPersistentObjectStore : public ObjectStore
{
public:
    /* Constructors & destructor */
    
    LocalPersistentObjectStore ();
    LocalPersistentObjectStore (const char*);
    virtual ~LocalPersistentObjectStore ();
    
    /* non-virtual member functions */
    
    ObjectState *allObjUids (const TypeName tName);
    
    /* The real interface */
    
    Boolean commit_state (const Uid&, const TypeName);
    
    ObjectState *read_state (const Uid&, const TypeName);
    ObjectState *read_uncommitted (const Uid&, const TypeName); 
    
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);
    
    /* These few should really be private */
    
    Boolean createHierarchy (const char*, int) const;

    char *genPathName (const Uid&, const TypeName) const;
    const TypeName type () const;

    const char *getStoreName () const;
    
private:
    /* Basic Object I/O */
    
    ObjectState *read_state (const Uid&, const TypeName, FileType);
    Boolean remove_state (const Uid&, const TypeName, FileType);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&, FileType);
    
    /* Underlying support mechanism */

#ifndef __linux__
    // nested types bug in g++ on linux
    Boolean closeAndUnlock (FdCache::cacheEntry *);
    FdCache::cacheEntry *openAndLock (const Uid&, const TypeName, const int);
#else
    Boolean closeAndUnlock (cacheEntry *);
    cacheEntry *openAndLock (const Uid&, const TypeName, const int);
#endif

    void setupStore ();
    
    long storeBlockSize;
    const char *storeName;
    Boolean storeValid;
};


// @NoRemote, @NoMarshall
class LPObjStoreSetUp
{
public:
    LPObjStoreSetUp ();
    ~LPObjStoreSetUp ();

    static ObjectStore* create (const char*);
    static void destroy (const char*);

private:
    static Boolean setUp;
    static StoreList* headOfList;
};

static LPObjStoreSetUp LPSetUp;


#include <ObjectStore/LPObjStore.n>

#endif
