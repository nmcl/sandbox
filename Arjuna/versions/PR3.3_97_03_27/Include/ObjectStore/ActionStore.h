/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionStore.h,v 1.4 1995/04/03 09:04:14 ngdp Exp $
 */

#ifndef ACTIONSTORE_H_
#define ACTIONSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef SHADOWSTORE_H_
#include <ObjectStore/ShadowStore.h>
#endif

// @NoRemote, @NoMarshall
class StoreList;


// @NoRemote, @NoMarshall
class ActionStore : public ShadowingStore
{
public:
    ActionStore (const char*);
    virtual ~ActionStore ();

    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
    
    virtual Boolean commit_state (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);

    virtual ObjectState *read_committed (const Uid&, const TypeName); 
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean remove_committed (const Uid&, const TypeName);
    virtual Boolean remove_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean write_committed (const Uid&, const TypeName,
				     const ObjectState&);
    virtual Boolean write_uncommitted (const Uid&, const TypeName,
				       const ObjectState&);
    
    virtual const TypeName type () const;
protected:
    ActionStore ();

    virtual Boolean lock (int, int) const;
    virtual Boolean unlock (int) const;
};

// @NoRemote, @NoMarshall
class ActionStoreSetUp : public ShadowingStoreSetUp
{
public:
    ActionStoreSetUp ();
    ~ActionStoreSetUp ();
protected:
    ActionStoreSetUp (SetUpInfo&);
};

static ActionStoreSetUp ActionSetUp;

#include <ObjectStore/ActionStore.n>

#endif
