/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedStore.h,v 1.2 1995/02/17 13:23:44 ngdp Exp $
 */

#ifndef HASHEDSTORE_H_
#define HASHEDSTORE_H_

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

#ifndef FRAGSTORE_H_
#  include <ObjectStore/FragStore.h>
#endif

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class ObjectState;

// @NoRemote, @NoMarshall
class StoreList;


// @NoRemote, @NoMarshall
class HashedStore : public FragmentedStore
{
public:
    /* Constructors & destructor */
    
    HashedStore (const char*);
    virtual ~HashedStore ();

    virtual Boolean allObjUids (const TypeName tName, ObjectState&,
				ObjectStore::StateStatus);

    virtual const TypeName type () const;
    
protected:
    HashedStore ();

    virtual char *genPathName (const Uid&, const TypeName,
			       ObjectStore::StateType) const;
    virtual Boolean supressEntry (const char *) const;
};


// @NoRemote, @NoMarshall
class HashedStoreSetUp : public FragmentedStoreSetUp
{
public:
    HashedStoreSetUp ();
    ~HashedStoreSetUp ();
protected:
     HashedStoreSetUp (SetUpInfo&);
    
};

static HashedStoreSetUp LHSetUp;

#include <ObjectStore/HashedStore.n>

#endif
