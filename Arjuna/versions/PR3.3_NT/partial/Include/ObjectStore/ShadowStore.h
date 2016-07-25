/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowStore.h,v 1.4 1995/02/17 13:23:52 ngdp Exp $
 */

#ifndef SHADOWSTORE_H_
#define SHADOWSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef FSSTORE_H_
#include <ObjectStore/FSStore.h>
#endif

// @NoRemote, @NoMarshall
class StoreList;


// @NoRemote, @NoMarshall
class ShadowingStore : public FileSystemStore
{
public:
    ShadowingStore (const char*);
    virtual ~ShadowingStore ();

    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
    
    virtual Boolean commit_state (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);

    virtual const TypeName type () const;

protected:
    ShadowingStore ();
    
    virtual char *genPathName (const Uid&, const TypeName,
			       ObjectStore::StateType) const;
    
private:
    virtual ObjectState *read_state (const Uid&, const TypeName,
				     ObjectStore::StateType);    
    virtual Boolean remove_state (const Uid&, const TypeName,
				  ObjectStore::StateType);
    virtual Boolean write_state (const Uid&, const TypeName,
				 const ObjectState&, ObjectStore::StateType);
    
    Boolean closeAndUnlock (int) const;
    int openAndLock (const char*, int, int) const;    
};

// @NoRemote, @NoMarshall
class ShadowingStoreSetUp : public FileSystemStoreSetUp
{
public:
    ShadowingStoreSetUp ();
    ~ShadowingStoreSetUp ();
protected:
     ShadowingStoreSetUp (SetUpInfo*);
private:
	static SetUpInfo* setup;
};

static ShadowingStoreSetUp ShadowingSetUp;

#include <ObjectStore/ShadowStore.n>

#endif
