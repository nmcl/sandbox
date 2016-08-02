/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedStore.h,v 1.2 1995/04/03 09:04:18 ngdp Exp $
 */

#ifndef MAPPEDOBJSTORE_H_
#define MAPPEDOBJSTORE_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef FSSTORE_H_
#  include <ObjectStore/FSStore.h>
#endif

#ifndef MAPPEDFILE_H_
#  include <ObjectStore/MappedFile.h>
#endif

/*
 * Change the following constants at compile time if you require more/less states
 */

// @NoRemote, @NoMarshall
class MappedStoreData;

// @NoRemote, @NoMarshall
class MappedStore : public FileSystemStore
{
public:
    MappedStore (const char * = "", Boolean = TRUE);
    virtual ~MappedStore ();

    virtual Boolean allObjUids (const TypeName, ObjectState&,
				ObjectStore::StateStatus);
    
    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);
    
    virtual Boolean commit_state (const Uid&, const TypeName);
    virtual Boolean fullCommitNeeded () const;
    
    virtual ObjectState *read_committed (const Uid&, const TypeName); 
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean remove_committed (const Uid&, const TypeName);
    virtual Boolean remove_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean write_committed (const Uid&, const TypeName,
				     const ObjectState&);
    virtual Boolean write_uncommitted (const Uid&, const TypeName,
				       const ObjectState&);

    const TypeName type () const;
    
protected: 
    virtual Boolean lock (int, int) const;
    virtual Boolean unlock (int) const;
 
private:
    virtual ObjectState *read_state (const Uid&, const TypeName,
                                     ObjectStore::StateType);    
    virtual Boolean remove_state (const Uid&, const TypeName,
                                  ObjectStore::StateType);
    virtual Boolean write_state (const Uid&, const TypeName,
                                 const ObjectState&, ObjectStore::StateType);    
    void deleteList ();
    ObjectStore *lookFor (const TypeName, Boolean = TRUE);

    MappedStoreData *head;
    Boolean writeShadow;
};

class MappedStoreSetUp : public FileSystemStoreSetUp
{
public:
    MappedStoreSetUp ();
    ~MappedStoreSetUp ();
};

#ifdef HAVE_MMAP
static MappedStoreSetUp MStoreSetUp;
#endif
#include <ObjectStore/MappedStore.n>

#endif
