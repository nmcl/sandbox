/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LVObjStore.h,v 
 */

#ifndef LVOBJSTORE_H_
#define LVOBJSTORE_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#include <Common/CommonT.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef SEMAPHORE_H_
#include "../../ClassLib/include/Semaphore.h"
#endif

#ifndef SHAREDSEG_H_
#include "../../ClassLib/include/SharedSeg.h"
#endif

#ifndef OBJSTORE_H_
#include <Arjuna/ObjStore.h>
#endif

const unsigned int numberOfStates = 200; // Maximum number of object states in volatile memory.
const unsigned int sizeOfStore = 1024000; // Maximum size of volatile object store.

class StoreData
{
public:
    StoreData ();
    ~StoreData ();

    Uid stateID;
    size_t stateSize;
    off_t offSet;
    Boolean isOriginal;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    StoreData* next;
};

class LocalVolatileObjectStore : public ObjectStore
{
public:
    LocalVolatileObjectStore ();
    virtual ~LocalVolatileObjectStore ();

    Boolean commit_state (const Uid&, const TypeName);
    ObjectState *read_state (const Uid&, const TypeName);
    ObjectState *read_uncommitted (const Uid&, const TypeName);
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);
    ObjectState *allObjUids (const TypeName);
    const TypeName type () const;
    void storeLocation (const char*);

    Boolean pack(Buffer&);
    Boolean unpack (Buffer);

private:
    ObjectState *read_state (const Uid&, const TypeName, FileType);
    Boolean remove_state (const Uid&, const TypeName, FileType);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&, FileType);

    Boolean read_data ();
    Boolean rename (const Uid&);

    Boolean openObjectStore ();
    Boolean closeObjectStore ();

    Boolean lockObjectStore ();
    Boolean unlockObjectStore ();

    Boolean openAndLock ();
    Boolean closeAndUnlock ();
    Boolean writeAndUnlock ();

    void deleteList ();
    size_t dataSegment ();
    size_t headerSize ();

    Boolean findOldState (StoreData*, const Uid&, Boolean,
			  const ObjectState&);
    Boolean findMemory (StoreData*, const ObjectState&,
			unsigned int&);

    TypeName OsType () const;

    Boolean storeValid, haveLock;
    SharedSegment* sharedSeg;
    Semaphore* sem;
    StoreData* head;
    unsigned int number_states, storeUsed;
    size_t cachedLength;
};

#endif
