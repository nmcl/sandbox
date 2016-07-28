/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VolatileStore.cc,v 1.18 1996/02/02 13:04:31 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef VOLATILESTORE_H_
#  include <ObjectStore/VolatileStore.h>
#endif

#ifndef PROCESSMUTEX_H_
#  include <Common/ProcessMutex.h>
#endif

#ifndef SHAREDSEG_H_
#  include "../../ClassLib/include/SharedSeg.h"
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

static osCreatorFunc createVSInstance;

#ifdef STRUCT_INIT_BUG
static SetUpInfo LVSetup = { 0, 0, 0, &createVSInstance, VolatileStoreType, 0 };
#else
static SetUpInfo LVSetup = { FALSE, FALSE, 0, &createVSInstance, VolatileStoreType, 0 };
#endif

ObjectStore* createVSInstance ( const char* root )
{
    return new VolatileStore(root);
}


VolatileStoreSetUp::VolatileStoreSetUp () : ObjectStoreSetUp(LVSetup)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "VolatileStoreSetUp::VolatileStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif

#ifdef GCC_STATIC_INIT_BUG
    if (!LVSetup._setUp)
    {
	LVSetup._new = &createVSInstance;
	LVSetup._tn = VolatileStoreType;
	LVSetup._setUp = TRUE;
	ObjectStore::addToList(&LVSetup);
    }
#endif 
}

VolatileStoreSetUp::~VolatileStoreSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "VolatileStoreSetUp::~VolatileStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif    
}

StoreData::StoreData ()
                      : stateID(NIL_UID),
			stateSize(0),
			offSet(0),
			isOriginal(TRUE),
			next(0)
{
}

StoreData::~StoreData ()
{
}

Boolean StoreData::pack (Buffer& buff) const
{
    Boolean result = buff.pack(stateSize) && buff.pack(offSet) && buff.pack(isOriginal);

    if (result)
	result = stateID.pack(buff);

    return result;
}

Boolean StoreData::unpack (Buffer& buff)
{
    Boolean result = buff.unpack(stateSize) && buff.unpack(offSet) && buff.unpack(isOriginal);

    if (result)
	result = stateID.unpack(buff);

    return result;
}


/*
 * Volatile object store implementation. Makes use of shared memory to store
 * object states. Note that object states exist in the volatile object store
 * for as long as the object store exists. This essentially means that they
 * exist for as long as an active process has a reference to the shared memory
 * segment.
 * If doShadow is FALSE we NEVER write shadow states - this may be a problem
 * but is here for efficiency...
 */

VolatileStore::VolatileStore ( const char *root, Boolean doShadow )
			     : storeValid(FALSE),
			       haveLock(FALSE),
			       writeShadow(doShadow),
			       number_states(0),
			       storeUsed(0),
			       lockCount(0),
			       head(0),
			       sharedSeg(0),
			       sem(0),
			       rootOfStore(clone(root))
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "VolatileStore::VolatileStore ( "
                 << ((doShadow == TRUE) ? "Shadows" : "NoShadows") << " )" << endl;
    debug_stream.unlock();
#endif

    if (openAndLockObjectStore() && unlockObjectStore())
        storeValid = TRUE;
    else
        storeValid = FALSE;
}

VolatileStore::~VolatileStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "VolatileStore::~VolatileStore ()\n" << flush;
    debug_stream.unlock();
#endif

    if (storeValid)
    {
        if (!closeAndUnlockObjectStore())
	    error_stream << WARNING << "VolatileStore::~VolatileStore - closeAndUnlock failed." << endl;

	deleteList();
	if (sem)
	    ProcessMutex::destroy(sem);
	if (sharedSeg)
	    delete sharedSeg;
    }

    if (rootOfStore)
#ifndef GCC_DELETE_BUG
	delete [] rootOfStore;
#else
    	delete rootOfStore;
#endif
}

void VolatileStore::deleteList ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void VolatileStore::deleteList ()\n" << flush;
    debug_stream.unlock();
    
#endif

    if (head)
    {
	StoreData* ptr = head->next;
	while (head)
	{
	    delete head;
	    head = ptr;
	    if (ptr)
		ptr = ptr->next;
	}
    }
}

Boolean VolatileStore::allTypes ( ObjectState& foundTypes ) const
{
    foundTypes.packMappedString("");
    return TRUE;
}

Boolean VolatileStore::openAndLockObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::openAndLockObjectStore ()\n" << flush;
    debug_stream.unlock();
    
#endif

    Boolean result = FALSE;

    if (lockObjectStore())
    {
        if (!sharedSeg)
	{
	    /* Give one shared segment per *different* installation. */
	    sharedSeg = new SharedSegment(typeToKey(rootOfStore), sizeOfStore);
	}

	if (!sharedSeg->isValid())
	{
	    error_stream << WARNING << "VolatileStore::openAndLockObjectStore - shared segment initialise failed." << endl;
	    storeValid = FALSE;
	}
	else
	    result = TRUE;
    }
    
    return result;
}

Boolean VolatileStore::closeAndUnlockObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::closeAndUnlockObjectStore ()"
		 << endl;
    debug_stream.unlock();
#endif

    if (sharedSeg)
    {
	delete sharedSeg;
	sharedSeg = 0;

	if (haveLock)
	    return unlockObjectStore();
	else
	    return TRUE;
    }

    return FALSE;
}

size_t VolatileStore::headerSize ()
{
    static size_t _size = 0;

    if (_size == 0)
    {
	Buffer buff;
	StoreData temp;

	temp.pack(buff);
	_size = buff.length();
    }

    return _size;
}

Boolean VolatileStore::lockObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::lockObjectStore ()\n" << flush;
    debug_stream.unlock();
#endif

    Boolean res = TRUE;
    
    if (!haveLock)
    {
        if (!sem)
	{
	    /* Give one semaphore per *different* installation. */
	    sem = ProcessMutex::create(typeToKey(rootOfStore));
	}
	
	if (sem && sem->wait() == Semaphore::SEM_LOCKED)
	{
	    haveLock = TRUE;
	    lockCount = 1;
	}
	else
	    res = FALSE;
    }
    else
    {
	lockCount++;
    }

    return res;
}

Boolean VolatileStore::unlockObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::unlockObjectStore ()\n" << flush;
    debug_stream.unlock();
#endif

    if ((!haveLock) || (!sem))
	return FALSE;

    if (--lockCount == 0)
    {
	sem->signal();
	haveLock = FALSE;
    }
    
    return TRUE;
}

Boolean VolatileStore::writeStates ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::writeStates ()\n" << flush;
    debug_stream.unlock();
#endif

    Boolean result = FALSE;

    if ((haveLock) && (storeValid))
    {
	StoreData* marker = head;
	long location = sizeof(number_states);

	if (sharedSeg->put(0, sizeof(number_states), (char*)&number_states) != sizeof(number_states))
	{
	    storeValid = FALSE;
	}
	else
	    if (number_states > 0)
	    {
		Buffer buff;
		size_t buffSize = 0;
	    
		/* Store object store management information first. */

		for (int i = 0; (i < number_states) && (storeValid); i++)
		{
		    storeValid = marker->pack(buff);
		    marker = marker->next;
		}
		
		buffSize = buff.length();
	    
		if (storeValid &&
		    (sharedSeg->put(location, sizeof(buffSize), (char*)&buffSize) ==  sizeof(buffSize)))
		{
		    location += sizeof(buffSize);
		    
		    if (sharedSeg->put(location, buffSize, buff.buffer()) != buffSize)
			storeValid = FALSE;
		}
		else
		    storeValid = FALSE;
	    }

	if (!storeValid)
	{
	    error_stream << WARNING
			 << "VolatileStore::writeStates - write failed"
			 << endl;
	    
	    if (!sharedSeg->isValid())
		error_stream << WARNING
			     << "VolatileStore - invalid shared segment"
			     << endl;
	}
	else
	    result = TRUE;
    }

    return result;
}

Boolean VolatileStore::readStates ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::readStates ()\n" << flush;
    debug_stream.unlock();
#endif

    if (!haveLock)
	return FALSE;
    
    SmAddr location = sizeof(number_states);
    Boolean result = FALSE;
    StoreData* marker = 0;

    if (storeValid)
    {
        deleteList();
	storeUsed = 0;

	/* new shared segment so no data to read. */

	if (sharedSeg->IsNew())
	{
	    number_states = 0;
	    result = TRUE;
	}
	else
	{
	    if (sharedSeg->get(0, sizeof(number_states), (char*)&number_states) != sizeof(number_states))
	        storeValid = FALSE;
	    else
	    {
		if (number_states > 0)
		{
		    size_t stateSize = 0;
		
		    if (sharedSeg->get(location, sizeof(stateSize), (char *)&stateSize) == sizeof(stateSize))
		    {
			char *buffer_data = ::new char[stateSize];
		    
			location += sizeof(stateSize);

			if (sharedSeg->get(location, stateSize, buffer_data) == stateSize)
			{
			    Buffer buff(stateSize, buffer_data);
			    
			    for (unsigned int i = 0; ((i < number_states) && (storeValid)); i++)
			    {
				if (!head)
				{
				    head = new StoreData;
				    marker = head;
				}
				else
				{
				    marker->next = new StoreData;
				    marker = marker->next;
				}
	    
				if ((storeValid = marker->unpack(buff)))
				{
				    storeUsed += marker->stateSize;
				}
			    }
			}
			else
			    storeValid = FALSE;
		    }
		    else
			storeValid = FALSE;
		    
		    if (!storeValid)
		    {
			error_stream << WARNING
				     << "VolatileStore::read_data - Could not unpack state" << endl;
		    }
		}
	    }
	}
    }

    if (storeValid)
	result = TRUE;
    else
    {
	error_stream << WARNING
		     << "VolatileStore::readStates - read failed" << endl; 
	if (!sharedSeg->isValid())
	    error_stream << WARNING
			 << "VolatileStore - invalid shared segment" << endl;
    }
    
    return result;
}

void VolatileStore::rename (const Uid& objUid)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void VolatileStore::rename ( " << objUid << " )" << endl;
    debug_stream.unlock();
#endif
    
    StoreData* marker = head;
    
    while (marker != 0)
    {
	if (marker->stateID == objUid)
	{
	    if (marker->isOriginal == FALSE)
	    {
		marker->isOriginal = TRUE;
		return;
	    }
	}

	marker = marker->next;
    }
}

void VolatileStore::removeElement (StoreData*& trail, StoreData*& toRemove)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void VolatileStore::removeElement ()" << endl;
    debug_stream.unlock();
#endif

    number_states--;
    if (number_states == 0)
	head = 0;
    else
    {
	if (trail != toRemove)
	    trail->next = toRemove->next;
	else
	    head = toRemove->next;
    }
    
    delete toRemove;
    toRemove = 0;
}

Boolean VolatileStore::findOldState (StoreData*& marker, const Uid& objUid,
				     Boolean fileType,
				     const ObjectState& state)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::findOldState ( " << objUid << ", "
                 << ((fileType == TRUE) ? "ORIGINAL" : "SHADOW") << " )"
		 << endl;
    debug_stream.unlock();
#endif

    Boolean found = FALSE, sizeOK = FALSE;
    StoreData* ptr = head;

    marker = ptr;
    if (head == 0)
	return FALSE;
    
    for (unsigned int i = 0; (!found) && (marker != 0); i++)
    {
	/*
	 * Check to see if old state is available to write over
	 * (must be correct size!)
	 */

	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	{
	    found = TRUE;

	    if (marker->stateSize >= state.size())
	        sizeOK = TRUE;
	    else
	    {
		/*
		 * The size if not correct, but can we extend this slot
		 * to accomodate the state?
		 */

		if (i < numberOfStates -1)
		{
		    if (marker->offSet + state.size() <= marker->next->offSet)
		        sizeOK = TRUE;
		}
	    }

	    /* found, but cannot re-use, so "zero" it. */

	    if (!sizeOK)
	        removeElement(ptr, marker);
	}

	if (!found)
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    return sizeOK;
}

Boolean VolatileStore::findMemory (StoreData*& marker,
				   const ObjectState& state, off_t& off_set)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean VolatileStore::findMemory ()" << endl;
    debug_stream.unlock();
#endif

    StoreData* trail = 0;
    Boolean found = FALSE;
    off_t prevSize = dataSegment();

    marker = head;
    
    if (head == 0)
	return TRUE;

    if ((storeUsed + state.size() <= sizeOfStore) || (number_states <= numberOfStates))
    {
	for (int i = 0; (!found) && (marker != 0); i++)
	{
	    if (marker->offSet - prevSize >= state.size())
	    {
		marker = trail;
		off_set = prevSize;
		found = TRUE;
	    }
	    else
	    {
		if (marker->next != 0)
		{
		    if (marker->next->offSet - marker->offSet - marker->stateSize >= state.size())
		    {
			off_set = marker->offSet + marker->stateSize;
			found = TRUE;
		    }
		}
		else
		{
		    found = TRUE;
		    off_set = marker->offSet + marker->stateSize;
		}
	    }

	    if (!found)
	    {
		prevSize = marker->offSet + marker->stateSize;
		trail = marker;
		marker = marker->next;
	    }
	}
    }
    else
        error_stream << WARNING << "VolatileStore - store full." << endl;
    
    return found;
}

ObjectStore::StateStatus VolatileStore::currentState ( const Uid& objUid,
						       const TypeName tn )
{
    ObjectStore::StateStatus status = ObjectStore::OS_UNKNOWN;
    StoreData *marker = 0;

    if (!storeValid || !tn)
	return ObjectStore::OS_UNKNOWN;

    if (!lockObjectStore())
	return ObjectStore::OS_UNKNOWN;

    if (!readStates())
    {
        unlockObjectStore();
	return ObjectStore::OS_UNKNOWN;
    }

    marker = head;

    while (marker != 0)
    {
	if (marker->stateID == objUid)
	{
	    if (!marker->isOriginal)
	    {
		status = ObjectStore::OS_UNCOMMITTED;
		break;
	    }
	    status = ObjectStore::OS_COMMITTED;
	}
	else
	    marker = marker->next;
    }

    unlockObjectStore();

    return status;
}

ObjectState* VolatileStore::read_state (const Uid& objUid, const TypeName tn,
					ObjectStore::StateType ft)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* VolatileStore::read_state ( " << objUid << ", "
                 << tn << ", " << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW")
		 << " )" << endl;
    debug_stream.unlock();
#endif

    if (!storeValid)
	return 0;
    
    Boolean found = FALSE, fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    StoreData *marker = 0, *ptr = 0;

    if (!tn)
    {
	error_stream << WARNING
		     << "VolatileStore::read_state - Object with uid "
		     << objUid << " has no TypeName\n"
		     << endl;
	return 0;
    }

    if ((ft != ObjectStore::OS_ORIGINAL) && (!writeShadow))
    {
        error_stream << WARNING
		     << "VolatileStore::read_state - Attempt to read shadow from non-shadowed store."
		     << endl;
	return 0;
    }

    if (!lockObjectStore())
	return 0;

    if (!readStates())
    {
        unlockObjectStore();
	return 0;
    }

    marker = head;
    ptr = marker;

    for (int i = 0; (!found) && (marker != 0); i++)
    {
	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	{
	    found = TRUE;
	    ptr = marker;
	}
	else
	    marker = marker->next;
    }

    if (found)
    {
	char *buffer = ::new char[(int)ptr->stateSize];
	
	if ((buffer != 0) &&
	    (sharedSeg->get(ptr->offSet, ptr->stateSize, buffer) != -1))
	{
	    unlockObjectStore();
	    return new ObjectState(objUid, tn, (int)ptr->stateSize, buffer);
	}
	else
	{
	    if (buffer != 0)
	    {
#ifdef GCC_ARRAY_DELETE_BUG
		delete buffer;
#else
		delete [] buffer;
#endif
	    }
	    
	    storeValid = FALSE;
	}
    }

    unlockObjectStore();

    return 0;
}

Boolean VolatileStore::commit_state (const Uid& objUid, const TypeName tn)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean VolatileStore::commit_state ( " << objUid << ", "
                 << tn << " )" << endl;
    debug_stream.unlock();
#endif

    if (!storeValid)
	return FALSE;

    if (!tn)
    {
	error_stream << WARNING
		     << "VolatileStore::commit_state - object with uid "
		     << objUid << " has no TypeName"
		     << endl;
	return FALSE;
    }

    if (!writeShadow)
        return TRUE;

    if (!lockObjectStore())
	return FALSE;

    if (!readStates())
    {
        unlockObjectStore();
	return FALSE;
    }

    StoreData *marker = head, *ptr = marker;
    Boolean found = FALSE;

    while ((marker != 0) && (!found))
    {
        /* Need to remove "old" original. */

	if ((marker->stateID == objUid) && (marker->isOriginal == TRUE))
	    found = TRUE;
	else
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    /* If found == TRUE then this is an "old" object. */

    if (found)
        removeElement(ptr, marker);

    rename(objUid);

    if (writeStates())
	return unlockObjectStore();
    else
    {
	(void) unlockObjectStore();
	return FALSE;
    }
}

Boolean VolatileStore::remove_state (const Uid& objUid, const TypeName name, 
				     ObjectStore::StateType ft)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean VolatileStore::remove_state ( " << objUid << ", "
                 << name << ", " << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW") << " )" << endl;
    debug_stream.unlock();
#endif

    if (!storeValid)
	return FALSE;

    if (!name)
    {
	error_stream << WARNING << "VolatileStore::remove_state - type() operation of object with uid "
		     << objUid << " returns NULL";
	return FALSE;
    }

    if ((ft != ObjectStore::OS_ORIGINAL) && (!writeShadow))
        return TRUE;

    if (!lockObjectStore())
        return FALSE;

    if (!readStates())
    {
        unlockObjectStore();
	return FALSE;
    }

    Boolean found = FALSE, fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    StoreData *marker = head, *ptr = marker;

    for (int i = 0; (!found) && (marker != 0); i++)
    {
	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	{
	    removeElement(ptr, marker);
	    found = TRUE;
	}
	else
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    if (writeStates())
	return unlockObjectStore();
    else
    {
	(void) unlockObjectStore();
	return FALSE;
    }
}

Boolean VolatileStore::write_committed ( const Uid& objUid, const TypeName tn,
					 const ObjectState& state )
{
    return write_state(objUid, tn, state, ObjectStore::OS_ORIGINAL);
}

Boolean VolatileStore::write_state ( const Uid& objUid, const TypeName tn,
				     const ObjectState& state,
				     ObjectStore::StateType ft )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean VolatileStore::write_state " << objUid << ", "
                 << tn << ", " << state << ", " << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW")
		 << " )" << endl;
    debug_stream.unlock();
#endif

    if (!storeValid)
	return FALSE;

    if (!tn)
    {
	error_stream << WARNING << "VolatileStore::write_state - Object with uid "
		     << objUid << " has no TypeName\n";
	return FALSE;
    }

    if ((ft == ObjectStore::OS_SHADOW) && (!writeShadow))
        return TRUE;

    if (!lockObjectStore())
	return FALSE;

    if (!readStates())
    {
        unlockObjectStore();
	return FALSE;
    }

    Boolean fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    Boolean useOld = FALSE;
    off_t off_set = 0;
    StoreData *marker = 0, *ptr = 0;
    
    if (findOldState(marker, objUid, fileType, state))
    {
	off_set = marker->offSet;
	useOld = TRUE;
    }
    else
    {
	/* No appropriate state found, so look for correct sized "hole". */

	if (!findMemory(marker, state, off_set))
	{
	    unlockObjectStore();
	    return FALSE;
	}
    }

    /*
     * Found correct location for "new" state.
     * Now move to data portion of object store.
     */

    if (off_set == 0)
	off_set = dataSegment();

    if (!useOld)
    {
        /* Create element to put state in. */

        ptr = new StoreData;
	number_states++;

	/* new shared segment, so this is first element in list. */

	if (head == 0)
	    head = ptr;
	else
	{
	    /* Goes before current first element. */

	    if (!marker)
	    {
	        ptr->next = head;
		head = ptr;
	    }
	    else
	    {
		/*
		 * Goes somewhere else in list.
		 * (After marker).
		 */

	        ptr->next = marker->next;
		marker->next = ptr;
	    }
	}

	marker = ptr;
    }

    /* Now fill in necessary data. */

    marker->stateID = objUid;
    marker->stateSize = state.size();
    marker->offSet = off_set;
    marker->isOriginal = fileType;
    storeUsed += state.size();

    /* Try to write it in to object store. */

    if (state.size() > 0)
    {
	if (sharedSeg->put(marker->offSet, state.size(), state.buffer()) == -1)
	{
	    error_stream << WARNING
			 << "VolatileStore::write_state failed" << endl;
	    
	    if (!sharedSeg->isValid())
		error_stream << WARNING
			     << "VolatileStore - invalid shared segment"
			     << endl;
	    
	    unlockObjectStore();
	    storeValid = FALSE;
	    return FALSE;
	}
    }

    if (writeStates())
	return unlockObjectStore();
    else
    {
	(void) unlockObjectStore();
	return FALSE;
    }
}

/*
 * Operations to allow packing and unpacking of object store.
 * Note: use with caution as the unpack will overwrite whatever
 * is currently in the existing object store.
 */

Boolean VolatileStore::pack (Buffer& buff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean VolatileStore::pack ()" << endl;
    debug_stream.unlock();
#endif

    if (!lockObjectStore())
	return FALSE;

    if (!readStates())
    {
        unlockObjectStore();
	return FALSE;
    }

    StoreData* marker = head;
    Boolean res = buff.pack(number_states);

    if ((res) && (number_states > 0))
    {
	while ((marker) && (res))
	{
	    res = marker->pack(buff);
	    marker = marker->next;
	}

	if (res)
	{
	    marker = head;

	    while ((marker) && (res))
	    {
		ObjectState* os;

		if (marker->isOriginal)
		    os = read_state(marker->stateID, OBJECTSTOREDIR_Q, ObjectStore::OS_ORIGINAL);
		else
		    os = read_state(marker->stateID, OBJECTSTOREDIR_Q, ObjectStore::OS_SHADOW);

		if (os)
		{
		    res = os->packInto(buff);
		    marker = marker->next;
		}
		else
		    res = FALSE;

		delete os;
	    }
	}
    }

    res = res && unlockObjectStore();

    return res;
}

Boolean VolatileStore::unpack (Buffer buff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean VolatileStore::unpack ()" << endl;
    debug_stream.unlock();
#endif

    if (!lockObjectStore())
	return FALSE;

    if (!readStates())
    {
        unlockObjectStore();
	return FALSE;
    }

    if (head)
	deleteList();

    StoreData* marker = 0;
    Boolean res = buff.unpack(number_states);

    if ((res) && (number_states > 0))
    {
	head = new StoreData;
	marker = head;

	for (int i = 0; ((i < number_states) && (res)); i++)
	{
	    res = marker->unpack(buff);

	    if (i+1 < number_states)
	    {
		marker->next = new StoreData;
		marker = marker->next;
	    }
	}

	if (res)
	{
	    marker = head;

	    while ((marker) && (res))
	    {
		ObjectState* os = new ObjectState;

		res = os->unpackFrom(buff);

		if (sharedSeg->put(marker->offSet, os->size(), os->buffer()) == -1)
		{
		    error_stream << WARNING << "VolatileStore - unpack failed" << endl;
		    if (!sharedSeg->isValid())
			error_stream << WARNING
				     << "VolatileStore - invalid shared segment" << endl;
		    unlockObjectStore();
		    storeValid = FALSE;
		    return FALSE;
		}

		marker = marker->next;
		delete os;
	    }
	}
    }

    if (res)
    {
	if (writeStates())
	    res = unlockObjectStore();
	else
	{
	    (void) unlockObjectStore();
	    res = FALSE;
	}
    }
    else
	unlockObjectStore();

    return res;
}


#ifdef NO_INLINES
#  define VOLATILESTORE_CC_
#  include "ObjectStore/VolatileStore.n"
#  undef VOLATILESTORE_CC_
#endif
