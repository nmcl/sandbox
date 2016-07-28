/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VolatileStore.h,v 1.5 1995/09/19 10:24:34 ngdp Exp $
 */

#ifndef LVOBJSTORE_H_
#define LVOBJSTORE_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif



/*
 * Change the following constants at compile time if you want a bigger volatile
 * object store and/or you require more/less states within that area.
 */

/*
 * Currently there is not way of turning on "shadowing" from the user
 * level, i.e., shadow states are not written to the object store.
 * The code for "shadowing" is here, but by default is not used. Some
 * means of allowing a user to select this at run-time will be added.
 */


const unsigned int numberOfStates = 200; // Maximum number of object states in volatile memory.
const unsigned int sizeOfStore = 1024000; // Maximum size of volatile object store.


// @NoRemote, @NoMarshall
class SharedSegment;

// @NoRemote, @NoMarshall
class ProcessMutex;


// @NoRemote, @NoMarshall
class StoreData
{
public:
    StoreData ();
    ~StoreData ();

    Uid     stateID;
    size_t  stateSize;
    off_t   offSet;
    Boolean isOriginal;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    StoreData *next;
};

// @NoRemote, @NoMarshall
class VolatileStore : public ObjectStore
{
public:
    VolatileStore (const char * = "", Boolean = TRUE);
    virtual ~VolatileStore ();

    virtual Boolean allObjUids (const TypeName, ObjectState&,
				ObjectStore::StateStatus);
    virtual Boolean allTypes (ObjectState&) const;
    virtual ObjectStore::StateStatus currentState (const Uid&, const TypeName);
    
    virtual Boolean hide_state (const Uid&, const TypeName);
    virtual Boolean reveal_state (const Uid&, const TypeName);
    
    virtual Boolean commit_state (const Uid&, const TypeName);

    virtual ObjectState* read_committed (const Uid&, const TypeName);
    virtual ObjectState* read_uncommitted (const Uid&, const TypeName);

    virtual Boolean remove_committed (const Uid&, const TypeName);
    virtual Boolean remove_uncommitted (const Uid&, const TypeName);

    virtual Boolean write_committed (const Uid&, const TypeName,
				     const ObjectState&);
    virtual Boolean write_uncommitted (const Uid&, const TypeName,
				       const ObjectState&);
    
    const TypeName type () const;
    const char *getStoreName () const;
    
    Boolean pack(Buffer&);
    Boolean unpack (Buffer);

protected:
    virtual Boolean supressEntry (const char *) const;

private:
    ObjectState* read_state (const Uid&, const TypeName, ObjectStore::StateType);
    Boolean remove_state (const Uid&, const TypeName, ObjectStore::StateType);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&,
			 ObjectStore::StateType);

    void rename (const Uid&);

    Boolean openAndLockObjectStore ();
    Boolean closeAndUnlockObjectStore ();
    Boolean lockObjectStore ();
    Boolean unlockObjectStore ();

    Boolean readStates ();
    Boolean writeStates ();

    void deleteList ();
    void removeElement (StoreData*&, StoreData*&);

    Boolean findOldState (StoreData*&, const Uid&, Boolean, const ObjectState&);
    Boolean findMemory (StoreData*&, const ObjectState&, off_t&);

    size_t dataSegment ();
    size_t headerSize ();

    Boolean        storeValid, haveLock, writeShadow;
    unsigned int   number_states, storeUsed, lockCount;
    StoreData*     head;
    SharedSegment* sharedSeg;
    ProcessMutex*     sem;
    char*          rootOfStore;
};


class VolatileStoreSetUp : public ObjectStoreSetUp
{
public:
    VolatileStoreSetUp ();
    ~VolatileStoreSetUp ();
};


static VolatileStoreSetUp LVSetUp;


#include <ObjectStore/VolatileStore.n>

#endif
