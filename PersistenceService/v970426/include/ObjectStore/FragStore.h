/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FragStore.h,v 1.6 1995/03/28 15:34:36 ngdp Exp $
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

#ifndef SHADOWSTORE_H_
#  include <ObjectStore/ShadowStore.h>
#endif

#ifndef FDCACHE_H_
#  include <ObjectStore/FdCache.h>
#endif


// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class ObjectState;

// @NoRemote, @NoMarshall
class StoreList;


// @NoRemote, @NoMarshall
class FragmentedStore : public ShadowingStore
{
public:
    /* Constructors & destructor */
    
    FragmentedStore (const char*);
    virtual ~FragmentedStore ();
    
    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);

    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);

    virtual Boolean commit_state (const Uid&, const TypeName);
    virtual Boolean fullCommitNeeded () const;
    
    virtual const TypeName type () const;

protected:
    FragmentedStore ();
    virtual char *genPathName (const Uid&, const TypeName,
			       ObjectStore::StateType) const;    
    virtual Boolean setupStore (const char*);    

private:
    /* Basic Object I/O */
    
    virtual ObjectState *read_state (const Uid&, const TypeName,
				     ObjectStore::StateType);
    virtual Boolean remove_state (const Uid&, const TypeName,
				  ObjectStore::StateType);
    virtual Boolean write_state (const Uid&, const TypeName,
				 const ObjectState&, ObjectStore::StateType);
    
    /* Underlying support mechanism */

#ifndef GCC_NESTED_TYPE_BUG
    Boolean closeAndUnlock (FdCache::cacheEntry *);
    FdCache::cacheEntry *openAndLock (const Uid&, const TypeName, int,
				      char * = 0);
#else
    Boolean closeAndUnlock (cacheEntry *);
    cacheEntry *openAndLock (const Uid&, const TypeName, int, char * = 0);
#endif

    long storeBlockSize;
    Boolean netByteOrder;
};


// @NoRemote, @NoMarshall
class FragmentedStoreSetUp : public ShadowingStoreSetUp
{
public:
    FragmentedStoreSetUp ();
    ~FragmentedStoreSetUp ();
protected:
     FragmentedStoreSetUp (SetUpInfo&);
};

static FragmentedStoreSetUp LPSetUp;


#include <ObjectStore/FragStore.n>

#endif
