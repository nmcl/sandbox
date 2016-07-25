/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStore.cc,v 1.2 1993/11/16 09:55:41 ngdp Exp $
 */

/*
 *
 * Simple Lock Store. To enable locks to be shared amongst processes
 * we compact them into an objectstate and store them in a lock store
 * when not in use.
 * Lock stores are organised based upon the type of an object (as determined
 * by the 'type()' operation). There is one store per type but each store
 * consists of multiple shared memory segments. There is one segment for
 * control information plus (at least one) other(s) for the actual states
 * representing the locks on any given object.
 * The actual lock segments are allocated only as needed - which may fail
 * in which case we attempt to share an earlier allocated segment if one
 * exists.
 *
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef LOCKSEG_H_
#  include "LockSeg.h"
#endif

#ifndef SHAREDSEG_H_
#  include "SharedSeg.h"
#endif

#ifndef LOCKSTORE_H_
#  include "LockStore.h"
#endif

static const char RCSid[] = "$Id: LockStore.cc,v 1.2 1993/11/16 09:55:41 ngdp Exp $";

static const int OBJECT_LIMIT = 1024;	        /* Max number of objects of a type */
static const size_t MIN_STATE_SIZE = 1024;	/* min size of a segment slot */
static const int UID_NAME_LEN = 48;		/* Uid as string no longer than this */

struct UidMap
{
    long slotNo;			/* Slot Uid is mapped to */
    long segmentNo;			/* Segment containing slot */
    char uidString[UID_NAME_LEN];	/* Uid as a string */
};

struct SegmentDescriptor
{
    int smId;				/* Shared memory id of segment */
    size_t slotSize;			/* size of a single slot */
};

struct MappingTable
{
    int segmentInitialised;
    int mapEntries;
    int segmentEntries;
    UidMap mapTable[OBJECT_LIMIT];		/* uid to segment & slot map */
    SegmentDescriptor segmentTable[SEGMENT_LIMIT];
};

/* The null map table entry template */

static UidMap nullMap = {-1, -1, ""};
static SegmentDescriptor nullSeg = {-1, 0};

/*
 * Public constructors and destructor 
 */

/*
 * Create the lock store. 
 * This creates or initialises the control segment but the actual
 * LockSegments are created lazily as needed. If control segment creation
 * is ok its current contents are loaded and those segments that already
 * exist (created in the system by others) are attached.
 * Note that creating a new SharedSegment implicitly sets its contents to 0.
 */


LockStore::LockStore ( const key_t key )
                     : SharedSegment(key, sizeof(struct MappingTable)),
		       lockSegKey(key),
		       mapTablePtr(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::LockStore(" << key << ")" << endl;
#endif

    if (isValid())
    {
	int i;

	mapTablePtr = (struct MappingTable *)getSegVaddr();
	
	if (mapTablePtr->segmentInitialised)
	{
	    for (i = 0; i < SEGMENT_LIMIT; i++)
	    {
		if (mapTablePtr->segmentTable[i].smId >= 0)
		    lockSegs[i] = new LockSegment(mapTablePtr->segmentTable[i].smId);
		else
		    lockSegs[i] = 0;
	    }
	}
	else
	{
	    /* initialise the null map entry here to ensure NIL_UID has been created */

	    ostrstream conv(nullMap.uidString, UID_NAME_LEN, ios::out);

	    nullMap.slotNo = -1;
	    conv << NIL_UID << ends;	/* printable version of NULL_UID */

	    for (i = 0; i < OBJECT_LIMIT; i++)
	    {
		mapTablePtr->mapTable[i] = nullMap;
	    }

	    for (i = 0; i < SEGMENT_LIMIT; i++)
	    {
		mapTablePtr->segmentTable[i] = nullSeg;
		lockSegs[i] = 0;
	    }
	    
	    mapTablePtr->segmentEntries = 0;
	    mapTablePtr->mapEntries = 0;
	    mapTablePtr->segmentInitialised = 1;
	}
    }
    else
	error_stream << WARNING 
		     << "LockStore::LockStore() failed to "
		     << "allocate shared memory for lock storage - "
		     << "(increase system limits?)"<< endl;
}

LockStore::~LockStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::~LockStore()" << endl;
#endif

    if (isValid())
    {
	for (int i = 0; i < mapTablePtr->segmentEntries; i++)
	    if (lockSegs[i] != 0)
	    {
		if (lockSegs[i]->usage() == 1)
		{
		    mapTablePtr->segmentTable[i] = nullSeg;
		}
	    
		delete lockSegs[i];
	    }
    }
}

/*
 * Public non-virtual functions
 */

/*
 * Emulate the read_state function of ObjectStore. Extracts an instance
 * of ObjectState out of the shared memory region which is managed as an 
 * instance of LockSegment and returns it to the user. 
 */

Boolean LockStore::read_state ( const Uid& u,
				const TypeName tName,
				ObjectState *& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::read_state(" << u << ", "
	         << tName << ")" << endl;
#endif

    state = 0;
    
    if (isValid())
    {
	long mapIndex = lookfor(u);

	if (mapIndex >= 0)
	{
	    long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;
	    long slotNo = mapTablePtr->mapTable[mapIndex].slotNo;

	    if (slotNo >= 0)
	    {
		LockSegment *useSeg = attachSegment(segmentIndex);
		
		if (useSeg != 0)
		    return useSeg->getState(u, tName, state, slotNo);
		else
		    error_stream << WARNING
				 << "LockStore::read_state() attach for segment index "
				 << segmentIndex << " failed" << endl;
	    } 
	}
	
	return TRUE;
    } 
    else
	error_stream << WARNING
		     << "LockStore::read_state() shared segment " << shmid()
		     << "is marked invalid" << endl;
    
    return FALSE;    
}

Boolean LockStore::remove_state ( const Uid& u, const TypeName tname )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream <<"LockStore::remove(" << u << ", " << tname << ")" << endl;
#endif

    if (isValid())
    {
	long mapIndex = lookfor(u);

	if (mapIndex >= 0)
	{
	    long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;	
	    long slotIndex = mapTablePtr->mapTable[mapIndex].slotNo;	    
	    LockSegment *useSeg = attachSegment(segmentIndex);
	    
	    mapTablePtr->mapTable[mapIndex].slotNo = -1;
	    
	    if (slotIndex >= 0)
	    {
		
		if (useSeg != 0)
		    return useSeg->freeSlot(slotIndex);
		else
		    error_stream << WARNING
				 << "LockStore::remove_state() segment attach for segment "
				 << segmentIndex << " failed" << endl;
	    }
	    else
		return TRUE;
	}
	return TRUE;
    }
    else
	 error_stream << WARNING
		      << "LockStore::remove_state() shared segment " << shmid()
		      << "is marked invalid" << endl;

    return FALSE;    
}

Boolean LockStore::write_committed ( const Uid& u,
				     const TypeName tName,
				     const ObjectState& state )
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream <<"LockStore::write_committed("  << u << ", "
	         << (void *)&state << ")" << endl;
#endif
    
    size_t stateSize = state.size();
    
    if (stateSize == 0)
    {
	error_stream << WARNING 
	             << "LockStore::write_state() : "
		     << "attempted write of empty state!\n";
	return FALSE;
    }
    
    if (isValid())
    {
	long mapIndex = insert(u);
	
	if (mapIndex >= 0)
	{
	    long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;
	    long slotIndex = mapTablePtr->mapTable[mapIndex].slotNo;
	    LockSegment *useSeg = 0;
	    
	    if (segmentIndex >= 0)
	    {
		/* A segment exists */

		useSeg = attachSegment(segmentIndex);
		
		if ((useSeg == 0) ||
		    (!useSeg->hasSpareSlots()) ||
		    (stateSize > mapTablePtr->segmentTable[segmentIndex].slotSize))
		{
		    /* But its slots are now too small or full */
		    
		    if ((useSeg != 0) &&
			(slotIndex >= 0))
		    {
			useSeg->freeSlot(slotIndex);
		    }
		    segmentIndex = allocateSegment(stateSize);
		}
	    } else
		segmentIndex = allocateSegment(stateSize);
	    
	    if (segmentIndex >= 0)
	    {
		mapTablePtr->mapTable[mapIndex].segmentNo = segmentIndex;
		useSeg = attachSegment(segmentIndex);
		
		if (useSeg != 0)
		{
		    if ((slotIndex >= 0) ||
			((slotIndex = useSeg->allocateSlot()) >= 0))
		    {
			mapTablePtr->mapTable[mapIndex].slotNo = slotIndex;
			
			if (!(useSeg->putState(slotIndex, state)))
			{
			    error_stream << WARNING 
					 << "LockStore::write_state() : "
					 << "failed to write into lock store" << endl;
			    return FALSE;
			}
			
			return TRUE;
		    }
		    else
			error_stream << WARNING 
				     << "LockStore::write_state() failed to allocate slot "
				     << endl;
		}
		else
		    error_stream << WARNING 
				 << "LocalStore::write_state() failed to attach segment "
				 << segmentIndex << endl;
	    }
	    else
		error_stream << WARNING 
			     << "LocalStore::write_state() failed to allocate segment "
			     << segmentIndex << endl; 
	}
	else
	    error_stream << WARNING 
			 << "LocalStore::write_state() map insert failed for object "
			 << u << " of type " << tName << endl;
    }
    else
	error_stream << WARNING 
		     << "LocalStore::write_state() segment with id " 
			 << shmid() << " is marked invalid" << endl;
    
    return FALSE;
}

/*
 * Table look up has determined which LockSegment to use. Attach it if needed
 */

LockSegment *LockStore::attachSegment ( long segIndex )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LockStore::attachSegment(" << segIndex << ")" << endl;
#endif

    /* Determine if another process has created a segment for use */

    if ((lockSegs[segIndex] == 0) && (mapTablePtr->segmentTable[segIndex].smId >= 0))
    {
	lockSegs[segIndex] = new LockSegment(mapTablePtr->segmentTable[segIndex].smId);
    }

    return lockSegs[segIndex];
}

long LockStore::allocateSegment ( size_t slotSize )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LockStore::allocateSegment(" << slotSize << ")" << endl;
#endif

    LockSegment *newSeg = 0;
    long i;

    for (i = 0; i < mapTablePtr->segmentEntries; i++)
    {
	if (mapTablePtr->segmentTable[i].slotSize > slotSize)
	{
	    newSeg = attachSegment(i);
	    if ((newSeg) &&
		(newSeg->hasSpareSlots()))
		    return i;
	}
    }
    
    if (newSeg == 0)
    {
	/* I must create one */
	
	size_t roundedSize = 2048;

	while (roundedSize < slotSize)
	    roundedSize <<= 1;
	
	newSeg = new LockSegment(lockSegKey, roundedSize);

	if (newSeg->isValid())
	{
	    lockSegs[i] = newSeg;
	    mapTablePtr->segmentTable[i].smId = newSeg->shmid();
	    mapTablePtr->segmentTable[i].slotSize = roundedSize;
	    return mapTablePtr->segmentEntries++;
	}
	else
	{
	    /* Segment creation failed */
		
	    delete newSeg;	/* it was invalid */
	    return -1;
	}
    }

    return -1;
}

/*
 * Map uid to slot number using mapping table in slot 0.
 * Assume control information is valid and up to date.
 */

long LockStore::lookfor ( const Uid& u ) const
{   
    long first = 0;
    long last = mapTablePtr->mapEntries;
    long ind;
    int res;

    char uidBuff[UID_NAME_LEN];
    ostrstream conv(uidBuff, UID_NAME_LEN, ios::out);
    
    conv << u << ends;	
    
    while (last > first)
    {
	ind = (last+first)/2;
	res = strcmp(mapTablePtr->mapTable[ind].uidString, uidBuff);
	
	if (res == 0)
	    return ind;

	if (res > 0)
	    last = ind;
	else
	    first = ind+1;
    }

    return -1;
}

/*
 * Insert uid into mapping table. Table is held sorted so that binary
 * search can be used in lookup routine.
 */

long LockStore::insert ( const Uid& u ) 
{
    long first = 0;
    long last = mapTablePtr->mapEntries;
    long ind;
    int res;

    char uidBuff[UID_NAME_LEN];
    ostrstream conv(uidBuff, UID_NAME_LEN, ios::out);
    
    conv << u << ends;	
    
    while (last > first)
    {
	ind = (last+first)/2;
	res = ::strcmp(mapTablePtr->mapTable[ind].uidString, uidBuff);
	
	if (res == 0)
	    return ind;
	if (res > 0)
	    last = ind;
	else
	    first = ind+1;
    }

    ind = mapTablePtr->mapEntries;
    while (ind > first)
    {
	mapTablePtr->mapTable[ind] = mapTablePtr->mapTable[ind-1];
	ind--;
    }

    ::strcpy(mapTablePtr->mapTable[first].uidString,uidBuff);
    mapTablePtr->mapTable[first].segmentNo = -1;
    mapTablePtr->mapTable[first].slotNo = -1;
    mapTablePtr->mapEntries++;

    return first;
}
