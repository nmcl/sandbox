/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LVObjStore.cc,v 
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
#include <System/string.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UTILITY_H_
#include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif

#ifndef LVOBJSTORE_H_
#include <Arjuna/LVObjStore.h>
#endif

#ifndef CONFIGURE_H_
#include <Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

StoreData::StoreData ()
                     : stateSize(0),
		       stateID(),
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
    Boolean result = FALSE;

    result = buff.pack(stateSize) && buff.pack(offSet) && buff.pack(isOriginal);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "packing " << stateSize << ", " << offSet << ", " << isOriginal << "\n" << flush;
#endif

    if (result)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "and " << stateID << "\n" << flush;
#endif
    
	result = stateID.pack(buff);
    }

    return result;
}

Boolean StoreData::unpack (Buffer& buff)
{
    Boolean result = FALSE;

    result = buff.unpack(stateSize) && buff.unpack(offSet) && buff.unpack(isOriginal);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "unpacked " << stateSize << ", " << offSet << ", " << isOriginal << "\n" << flush;
#endif

    if (result)
    {
	result = stateID.unpack(buff);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "and " << stateID << "\n" << flush;
#endif
    }

    return result;
}

/*
 * Volatile object store implementation. Makes use of shared memory to store object states.
 * Note that object states exist in the volatile object store for as long as the object     
 * store exists. This essentially means that they exist for as long as an active process has
 * a reference to the shared memory segment.                                                
 */

LocalVolatileObjectStore::LocalVolatileObjectStore ()
                                                   : storeValid(TRUE),
						     sem(0),
						     sharedSeg(0),
						     head(0),
						     number_states(0),
						     storeUsed(0),
						     haveLock(FALSE),
						     cachedLength(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalVolatileObjectStore::LocalVolatileObjectStore ()\n" << flush;
#endif

/*
 * Need to create object store now or attach to already existing one to allow
 * cadaver record to work.
 */

    (void) openAndLock();
    (void) unlockObjectStore();
}

LocalVolatileObjectStore::~LocalVolatileObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalVolatileObjectStore::~LocalVolatileObjectStore ()\n" << flush;
#endif

    // should delete object states when object store instance goes out of scope.

    if (storeValid)
    {
	if (!closeAndUnlock())
	    error_stream << WARNING << "LocalVolatileObjectStore::~LocalVolatileObjectStore - Could not close object store cleanly.\n" << flush;

	deleteList();

	if (sem)
	    delete sem;

	if (sharedSeg)
	    delete sharedSeg;
    }
    else
	 error_stream << WARNING << "LocalVolatileObjectStore::~LocalVolatileObjectStore - Invalid object store, so will not attempt to close it\n" << flush;   
}

void LocalVolatileObjectStore::deleteList ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void LocalVolatileObjectStore::deleteList ()\n" << flush;
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

	head = 0;
    }
}

Boolean LocalVolatileObjectStore::openObjectStore ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::openObjectStore ()\n" << flush;
#endif

    if (!haveLock)
	return FALSE;

    if (!sharedSeg)
    {
	sharedSeg = new SharedSegment(typeToKey(OsType()), sizeOfStore);
    }

    return TRUE;
}

Boolean LocalVolatileObjectStore::closeObjectStore ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::closeObjectStore ()\n" << flush;
#endif

    if (sharedSeg)
    {
	delete sharedSeg;
	sharedSeg = 0;
    }

    return TRUE;
}

size_t LocalVolatileObjectStore::headerSize ()
{
    if (!cachedLength)
    {
	Buffer buff;
	StoreData temp;

	temp.pack(buff);

	cachedLength = buff.length();
    }

    return cachedLength;
}

size_t LocalVolatileObjectStore::dataSegment ()
{
    return sizeof(number_states) + (headerSize() * numberOfStates);
}

Boolean LocalVolatileObjectStore::lockObjectStore ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::lockObjectStore ()\n" << flush;
#endif

    if (!sem)
    {
	sem = new Semaphore(typeToKey(OsType()));
    }

    sem->wait();
    haveLock = TRUE;
    
    return TRUE;
}

Boolean LocalVolatileObjectStore::unlockObjectStore ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::unlockObjectStore ()\n" << flush;
#endif

    if (!sem)
	return FALSE;

    sem->signal();
    haveLock = FALSE;

    return TRUE;
}

Boolean LocalVolatileObjectStore::openAndLock ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::openAndLock ()\n" << flush;
#endif

    if (lockObjectStore())
    {
	if (openObjectStore())
	{
	    if (read_data())
		return TRUE;
	    else
	    {
		error_stream << WARNING << "LocalVolatileObjectStore::openAndLock - Could not read data from object store\n" << flush;   
		(void)closeAndUnlock();
	    }
	}
	else
	    error_stream << WARNING << "LocalVolatileObjectStore::openAndLock - Could not open object store\n" << flush;
    }
    else
	error_stream << WARNING << "LocalVolatileObjectStore::openAndLock - Could not lock object store\n" << flush; 

    storeValid = FALSE;

    return FALSE;
}

Boolean LocalVolatileObjectStore::closeAndUnlock ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::closeAndUnlock ()\n" << flush;
#endif

    Boolean opcode;

    opcode = closeObjectStore() && unlockObjectStore();
    
    return opcode;
}

Boolean LocalVolatileObjectStore::writeAndUnlock ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::writeAndUnlock ()\n" << flush;
#endif

    if ((head != 0) && (haveLock))
    {
	StoreData* marker = head;
	long location = sizeof(number_states);

	if (sharedSeg->put(0, sizeof(number_states), (char*)&number_states) == -1)
	{
	    error_stream << WARNING << "LocalVolatileObjectStore::writeAndUnlock - Could not write number of states to object store\n" << flush; 
	    storeValid = FALSE;
	    return FALSE;
	}

	for (unsigned int i = 0; (i < number_states) && (marker != 0); i++) // Store object store management information first.
	{
	    Buffer buff(headerSize());

	    marker->pack(buff);
	    if (sharedSeg->put(location, headerSize(), buff.buffer()) == -1)
	    {
		error_stream << WARNING << "LocalVolatileObjectStore::writeAndUnlock - Something bad happened while attempting to write state\n" << flush; 

		storeValid = FALSE;
		return FALSE;
	    }

	    location += headerSize();
	    marker = marker->next;
	}
    }

    return unlockObjectStore();
}

Boolean LocalVolatileObjectStore::read_data ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::read_data ()\n" << flush;
#endif

    SmAddr location = sizeof(number_states);
    Boolean opcode = TRUE;
    StoreData* marker = 0;

    if (!haveLock)
	return FALSE;

    deleteList();

    storeUsed = 0;

    if (sharedSeg->IsNew())  // new shared segment so no data to read.
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Shared segment created by this call\n" << flush;
#endif
	number_states = 0;
	return TRUE;
    }

    if (sharedSeg->get(0, sizeof(number_states), (char*)&number_states) == -1)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::read_data - Could not read number of states from object store\n" << flush; 
	storeValid = FALSE;

	return FALSE;
    }

    for (unsigned int i = 0; ((i < number_states) && (opcode)); i++)
    {
	char* buffer_data = new char[headerSize()];

	if (sharedSeg->get(location, headerSize(), buffer_data) == -1)
	{
	    error_stream << WARNING << "LocalVolatileObjectStore::read_data - Something bad happened while reading state\n" << flush; 
	    storeValid = FALSE;
	    opcode = FALSE;
	}
	else
	{
	    Buffer buff(headerSize(), buffer_data);

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
	    
	    if (marker->unpack(buff))
	    {
		storeUsed += marker->stateSize;
		location += headerSize();
	    }
	    else
	    {
		error_stream << WARNING << "LocalVolatileObjectStore::read_data - Could not unpack state\n" << flush;
		opcode = FALSE;
	    }
	}
    }

    return opcode;
}

Boolean LocalVolatileObjectStore::rename (const Uid& objUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::rename ()\n" << flush;
#endif
    
    StoreData* marker = head;
    Boolean isChanged = FALSE;

    while ((marker != 0) && (!isChanged))
    {
	if (marker->stateID == objUid)
	{
	    marker->isOriginal = TRUE;
	    isChanged = TRUE;
	}
	else
	    marker = marker->next;
    }

    if (!isChanged)
	error_stream << WARNING << "LocalVolatileObjectStore::rename - Could not find state to rename!!\n" << flush; 

    return isChanged;
}

Boolean LocalVolatileObjectStore::commit_state (const Uid& objUid, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::commit_state\n" << flush;
#endif

    StoreData *marker, *ptr;
    Boolean found = FALSE;

    if (!storeValid)
	return FALSE;

    if (!tn)
    {
	error_stream << FATAL << "LocalVolatileObjectStore::commit_state - object with uid "
	    << objUid << " has no TypeName\n";
	ArjunaFatal();
    }

    if (!openAndLock())
	return FALSE;

    marker = head;
    ptr = marker;

    while ((marker != 0) && (!found))
    {
	if ((marker->stateID == objUid) && (marker->isOriginal))
	    found = TRUE;
	else
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    if (found) // if (!found) then is new object
    {
	if (marker != head)
	    ptr->next = marker->next;
	else
	    head = marker->next;
    
	if (marker)
	{
	    delete marker;
	    marker = 0;
	}

	number_states--;
    }
    
    if (!rename(objUid))
    {
	error_stream << WARNING << "LocalVolatileObjectStore::commit_state - Could not find shadow state\n" << flush;
	(void)unlockObjectStore();
	return FALSE;
    }
    else
	return writeAndUnlock();
}

ObjectState* LocalVolatileObjectStore::read_state (const Uid& objUid, const TypeName tn)
{
    return read_state(objUid, tn, ORIGINAL);
}

ObjectState* LocalVolatileObjectStore::read_state (const Uid& objUid, const TypeName tn,
						   FileType ft)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* LocalVolatileObjectStore::read_state\n" << flush;
#endif
    
    ObjectState* new_image = 0;
    Boolean found = FALSE, fileType = ((ft == ORIGINAL) ? TRUE : FALSE);
    StoreData *marker, *ptr;

    if (!storeValid)
	return new_image;

    if (!tn)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::read_state - Object with uid "
	    << objUid << " has no TypeName\n";
	return 0;
    }

    if (!openAndLock())
	return new_image;

    // Is open && locked && successfully read.

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
	new_image = new ObjectState(objUid, tn, (int)ptr->stateSize,
				    new char[(int)ptr->stateSize]);

	if (sharedSeg->get(ptr->offSet, ptr->stateSize, new_image->buffer()) != -1)
	{
	    (void) unlockObjectStore();
	    return new_image;
	}
	else
	    storeValid = FALSE;
    }

    (void) unlockObjectStore();

    if (new_image)
	delete new_image;

    return 0;
}

ObjectState* LocalVolatileObjectStore::read_uncommitted (const Uid& objUid, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* LocalVolatileObjectStore::read_uncommitted\n" << flush;
#endif

    return read_state(objUid, tn, SHADOW);
}

Boolean LocalVolatileObjectStore::remove_state (const Uid& objUid, const TypeName tn)
{
    return remove_state(objUid, tn, ORIGINAL);
}

Boolean LocalVolatileObjectStore::remove_state ( const Uid& objUid, const TypeName name, 
						FileType ft)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::remove_state\n" << flush;
#endif

    Boolean found = FALSE, fileType = ((ft == ORIGINAL) ? TRUE : FALSE), opcode = FALSE;
    StoreData *marker, *ptr;

    if (!storeValid)
	return FALSE;

    if (!name)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::remove_state - type() operation of object with uid "
	    << objUid << " returns NULL";
	return FALSE;
    }

    if (!openAndLock())
	return FALSE;

    // Is open && locked && successfully read.
    
    marker = head;
    ptr = marker;

    for (int i = 0; (!found) && (marker != 0); i++)
    {
	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	    found = TRUE;
	else
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    if (found)
    {
	ptr->next = marker->next;

	if (marker)
	{
	    delete marker;
	    marker = 0;
	}

	number_states--;
	opcode = writeAndUnlock();
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Could not find state to remove\n" << flush;
#endif
	opcode = unlockObjectStore();
    }
	
    return opcode;
}

Boolean LocalVolatileObjectStore::remove_uncommitted (const Uid& objUid, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::remove_uncommitted\n" << flush;
#endif

    return remove_state(objUid, tn, SHADOW);
}

Boolean LocalVolatileObjectStore::write_committed (const Uid& objUid, const TypeName tn,
						   const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::write_committed\n" << flush;
#endif

    return write_state(objUid, tn, state, ORIGINAL);
}

Boolean LocalVolatileObjectStore::write_state (const Uid& objUid, const TypeName tn,
					       const ObjectState& state)
{
    return write_state(objUid, tn, state, SHADOW);
}

Boolean LocalVolatileObjectStore::findOldState (StoreData* marker, const Uid& objUid,
						Boolean fileType, const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::findOldState\n" << flush;
#endif

    Boolean found = FALSE;
    StoreData* ptr = marker;

    for (unsigned int i = 0; (!found) && (marker != 0); i++)
    {
	// Check to see if old state is available to write over (must be correct size!)

	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	{
	    if (marker->stateSize >= state.size())
		found = TRUE;
	    else
	    {
		if (i < numberOfStates -1)
		{
		    if (marker->offSet + state.size() <= marker->next->offSet)
			found = TRUE;
		}
	    }
	    // found, but cannot re-use, so "zero" it.

	    if (!found)
	    {
		ptr->next = marker->next;

		if (marker)
		{
		    delete marker;
		    marker = 0;
		}

		marker = ptr->next;
		number_states--;

		if (number_states == 0)
		    head = 0;
	    }
	}

	ptr = marker;
	marker = marker->next;
    }

    return found;
}

Boolean LocalVolatileObjectStore::findMemory (StoreData* marker, const ObjectState& state,
					      unsigned int& off_set)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::findMemory\n" << flush;
#endif

    Boolean found = FALSE;

    if (head == 0)
	return TRUE;

    if ((storeUsed + state.size() <= sizeOfStore) || (number_states <= numberOfStates))
    {
	for (int i = 0; (!found) && (marker != 0); i++)
	{
	    if (marker->offSet - dataSegment() >= state.size())
	    {
		found = TRUE;
		off_set = 0;
	    }
	    else
	    {
		if (marker->next != 0)
		{
		    if (marker->offSet + marker->stateSize - marker->next->offSet >= state.size())
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
		marker = marker->next;
	}
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Virtual object store full. Cannot write state\n" << flush;
#endif
	(void)unlockObjectStore();
	return FALSE;
    }

    return found;
}

Boolean LocalVolatileObjectStore::write_state (const Uid& objUid, const TypeName tn,
					       const ObjectState& state, FileType ft = SHADOW)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::write_state\n" << flush;
#endif

    Boolean fileType = ((ft == ORIGINAL) ? TRUE : FALSE);
    Boolean useOld = FALSE;
    unsigned int off_set = 0;
    StoreData *marker, *ptr;

    if (!storeValid)
	return FALSE;

    if (!tn)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::write_state - Object with uid "
	    << objUid << " has no TypeName\n";
	return FALSE;
    }

    if (state.notempty() == FALSE)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::write_state() - empty ObjectState!!";
	return FALSE;
    }

    if (!openAndLock())
	return FALSE;

    // Is open && locked && successfully read.

    marker = head;
    if (findOldState(marker, objUid, fileType, state))
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Found old state to write over\n" << flush;
#endif

	off_set = marker->offSet;
	useOld = TRUE;
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Could not find old state to write over\n" << flush;
#endif

	// No appropriate state found, so look for correct sized "hole".

	marker = head;
	if (!findMemory(marker, state, off_set))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	    debug_stream << "Could not find memory slot to take new state\n" << flush;
#endif
	    (void)unlockObjectStore();
	    return FALSE;
	}
    }

    // Found correct location for "new" state.

    if (off_set == 0)
	off_set = dataSegment(); // Move to data portion of object store

    if (!useOld)
    {
	if (head == 0)                  // new shared segment, so this is first element.
	{
	    head = new StoreData;
	    marker = head;
	}
	else
	{
	    ptr = marker->next;
	    marker->next = new StoreData;
	    marker = marker->next;
	    marker->next = ptr;
	}
    }

    marker->stateID = *new Uid(objUid);
    marker->stateSize = state.size();
    marker->offSet = off_set;
    marker->isOriginal = fileType;
    number_states++;
    storeUsed += state.size();

    if (sharedSeg->put(marker->offSet, state.size(), state.buffer()) == -1)
    {
	error_stream << WARNING << "LocalVolatileObjectStore::write_state - Something bad happened while writing object state!";
	(void)unlockObjectStore();
	return FALSE;
    }

    return writeAndUnlock();
}

ObjectState* LocalVolatileObjectStore::allObjUids (const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* LocalVolatileObjectStore::allObjUids\n" << flush;

#endif

    // This operation does not make sense for the volatile object store.

    return 0;
}

void LocalVolatileObjectStore::storeLocation (const char* location) {}

/*
 * Dummy OsType() operation to obtain unique semaphore and shared memory segment keys
 */

TypeName LocalVolatileObjectStore::OsType () const
{
    return OBJECTSTOREDIR_Q;
}

const TypeName LocalVolatileObjectStore::type () const
{
    return "LocalVolatileObjectStore";
}

/*
 * Operations to allow packing and unpacking of object store.
 * Note: use with caution as the unpack will overwrite whatever
 * is currently in the existing object store.
 */

Boolean LocalVolatileObjectStore::pack (Buffer& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::pack\n" << flush;
#endif

    if (!openAndLock())
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Could not open and lock object store to pack\n" << flush;
#endif
	return FALSE;
    }

    StoreData* marker = head;
    Boolean res;

    res = buff.pack(number_states);

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
		TypeName dummyName = OBJECTSTOREDIR_Q;

		if (marker->isOriginal)
		    os = read_state(marker->stateID, dummyName, ORIGINAL);
		else
		    os = read_state(marker->stateID, dummyName, SHADOW);

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

    res = closeAndUnlock();

    return res;
}

Boolean LocalVolatileObjectStore::unpack (Buffer buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean LocalVolatileObjectStore::unpack\n" << flush;
#endif

    if (!lockObjectStore())
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "Could not lock object store to over-write\n" << flush;
#endif
	return FALSE;
    }

    if (head)
	deleteList();

    StoreData* marker = 0;
    Boolean res = buff.unpack(number_states) && openObjectStore();

    if ((res) && (number_states > 0))
    {
	head = new StoreData;
	marker = head;

	for (unsigned int i = 0; ((i < number_states) && (res)); i++)
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
		    error_stream << WARNING <<
			"LocalVolatileObjectStore::write_state - Something bad happened while writing object state!";

		    (void)unlockObjectStore();
		    return FALSE;
		}

		marker = marker->next;
		delete os;
	    }
	}
    }

    if (res)
	res = writeAndUnlock();
    else
	(void)unlockObjectStore();

    return res;
}
	
