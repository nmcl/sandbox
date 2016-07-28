/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockSeg.cc,v 1.14 1995/10/02 15:13:28 ngdp Exp $
 */

/*
 * Simple Lock Store Segment. Layered on top of SharedSegment it manages
 * a shared segment as an array of slots. Slot 0 is reserved for a table
 * that maps unique ids (Uid's) to slots. This ensures that all object
 * states with the same uid end up in the same slot.
 * Each slot contains an ObjectState preceeded by a structure that contains 
 * a red zone and the size of the state.
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

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
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

static const char RCSid[] = "$Id: LockSeg.cc,v 1.14 1995/10/02 15:13:28 ngdp Exp $";

/* 
 * Managing shared memory dynamically is hard - particularly the table in 
 * slot 0 which can grow and shrinks. Treat it as a fixed size array instead
 * for simplicity.
 */

static const int MAX_SEGMENT_SIZE = 768 * 1024;
static const int MAX_SLOTS = 768;
static const char IS_ALLOC = 1;
static const char IS_FREE = 0;

/* The structure that sits in slot 0 */

static struct SegmentZero
{
    size_t slotSize;			/* physical size of a single slot usable size is LESS */
    long numSlots;			/* number in region */
    long slotsInUse;			/* number actually in use */
    char allocMap[MAX_SLOTS];		/* allocation map */
} segZero;

static const struct SlotHeader
{
    long guard;
    size_t stateSize;
    unsigned long chkSum;
} redZone = { 0x12357ace, 0, 0 };



/*
 * Public constructors and destructor 
 */

/*
 * Lock segment constructor. Since this looks like a shared segment it has
 * two parameters BUT we ignore the key_t parameter here as it is not needed
 * NOTE: slotSize is the usable size needed by the caller - we add a slop factor
 * for the header we need in addition
 */

LockSegment::LockSegment ( key_t, size_t slotSize )
                         : BaseSegment()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::LockSegment("
	         << slotSize << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    /* 
     * Slot 0 contains the control info (allocation map
     * and header). Ensure the requested slot size will hold it. If not
     * ignore the requested size and use our own.
     */

    size_t minSize = sizeof(struct SegmentZero);
    size_t allocSize = ((slotSize + sizeof(SlotHeader)) > minSize ? slotSize + sizeof(SlotHeader) : minSize);
    long slotCount = MAX_SEGMENT_SIZE / allocSize;
    

    if (slotCount > MAX_SLOTS)
	slotCount = MAX_SLOTS;
    
    /* Note that new segments are already initialised to 0 */

    if (smInit(IPC_PRIVATE, (unsigned int)(allocSize*slotCount)) != 0)
    {
	/* Initialise segment 0 */

	segZero.slotSize = allocSize;
	segZero.numSlots = slotCount;
	segZero.slotsInUse = 1;

	::memset(segZero.allocMap, 0, sizeof(segZero.allocMap));
	
	/* slot 0 must be marked as allocated */

	segZero.allocMap[0] = IS_ALLOC;

	put(0, sizeof(segZero), (const char *)&segZero);

	/* Now mark the header in each slot */

	for (int i = 1; i < slotCount; i++)
	{
	    long offset = i * allocSize;
	    put(offset, sizeof(SlotHeader), (const char *)&redZone);
	}
    }    
}

/*
 * Create new LockSegment manager for existing segment
 */

LockSegment::LockSegment ( int lockSegId )
                         : BaseSegment(lockSegId)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::LockSegment(" << lockSegId
	         << ")" << endl;
    debug_stream.unlock();
    
#endif

}

/*
 * Destructor - does nothing except print debugging information if requested
 */

LockSegment::~LockSegment ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::~LockSegment()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Public non-virtual functions
 */

Boolean LockSegment::getState ( const Uid& u,
				const TypeName tName,
				ObjectState *&state,
				long slotNo ) 
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::getState(" << u << ", " 
	         << tName << ")" << endl;
    debug_stream.unlock();
    
#endif

    state = 0;
    
    if (isValid())
    {
	char buff[256];
	ostrstream temp(buff,256);

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	checkIfConsistent();
	
	if (slotNo > 0)
	{
	    long offset = slotNo * segZero.slotSize;
	    SlotHeader b = redZone;
	    
	    /* Get size of saved state */

	    if ((get(offset, sizeof(SlotHeader), (char *)&b) == sizeof(SlotHeader)) &&
		(b.guard == redZone.guard))
	    {
		if (b.stateSize > 0)
		{
		    char *buffer = new char[b.stateSize];
		    long chkSum = 0;
		    long sizeRead = 0;
		    
		    if ((buffer != 0) &&
			((sizeRead = get(offset+sizeof(SlotHeader), b.stateSize, buffer)) == b.stateSize))
		    {
			chkSum = checkSum(buffer, b.stateSize);
			
			if (b.chkSum == chkSum)
			{
			    state = new ObjectState(u, tName, b.stateSize, buffer);
			    return TRUE;
			}
		    }

		    /* get here -> error */

		    temp << "LockSegment::getState(): ";
			
		    if (buffer != 0)
		    {
			if (b.chkSum != chkSum)
			{
			    temp << "corrupt state in slot - expecting: " << b.chkSum << " got : "
				 << chkSum << " for slot: ";
			}
			else
			{
			    temp << "incorrect size read - expecting: " << b.stateSize << " got : "
				 << sizeRead << " for slot: ";
			}
			    
#ifdef GCC_ARRAY_DELETE_BUG
			    delete buffer;
#else
			    delete [] buffer;
#endif
		    }
		    else
			temp << "out of memory reading slot : ";

		    temp << slotNo << " for segment: " << shmid() << ends;
		    addError(new Error(FAC_OBJECT_STORE, OS_CORRUPT, buff));

		    return FALSE;
		} 
		else if (b.stateSize == 0)
		{
		    return TRUE;
		}
	    }
	    else
	    {
		/* State header is corrupt */
		temp << "LockSegment::getState() : corrupt state header in slot: " << slotNo
		     << " for segment: " << shmid()
		     << hex << " guard: " << b.guard << dec << " size: " << b.stateSize << ends;
		
		addError(new Error(FAC_OBJECT_STORE, OS_CORRUPT, buff));
	    }
	}
	else
	{
	    temp << "LockSegment::getState() : invalid slot number " << slotNo
		 << ends;
	    addError(new Error(FAC_OBJECT_STORE, LS_BAD_SLOT,buff));
	}
    }
    
    return FALSE;
}

/*
 * remove for the LockSegment class simply clears the slot
 * size to zero. This effectively deletes the objectstate.
 * We leave the mapping table intact since Arjuna currently will
 * write a new state for the same uid almost immediately after the
 * delete.
 */ 

Boolean LockSegment::freeSlot ( long slotNo )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream <<"LockSegment::freeSlot(" << slotNo << ")" << endl;
    debug_stream.unlock();
    
#endif

    if (getSegVaddr() != 0)
    {
	char buff[256];
	ostrstream temp(buff,256);    

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	checkIfConsistent();
	
	if (slotNo > 0)
	{
	    long offset =  slotNo * segZero.slotSize;
	    SlotHeader b = redZone;

	    if ((get(offset, sizeof(SlotHeader), (char *)&b) == sizeof(SlotHeader)) &&
		(b.guard == redZone.guard))
	    {
		b = redZone;
		put(offset, sizeof(SlotHeader), (char *)&b);
		segZero.allocMap[slotNo] = IS_FREE;
		segZero.slotsInUse--;
		
		put(0, sizeof(segZero), (char *)&segZero);
		return TRUE;
	    }
	    else
	    {
		temp << "LockSegment::freeSlot() : corrupt state in slot: " << slotNo
		     << " for segment: " << shmid() << ends;
		addError(new Error(FAC_OBJECT_STORE, OS_CORRUPT, buff));
	    }
	    
	}
	else
	{
	    temp << "LockSegment::freeSlot() : invalid slot number " << slotNo
		 << ends;
	    addError(new Error(FAC_OBJECT_STORE, LS_BAD_SLOT,buff));
	}
    }
   
    return FALSE;
}

/*
 * Put an ObjectState into a slot
 */

Boolean LockSegment::putState ( long slotNo,
			        const ObjectState& state )
{
#ifdef DEBUG
    debug_stream.lock();
    
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream <<"LockSegment::putState("  << slotNo << ", "
	         << (void *)&state << ")" << endl;
    debug_stream.unlock();
    
#endif

    size_t stateSize = state.size();

    if (stateSize == 0)
    {
	addError(new Error(FAC_OBJECT_STORE, OS_EMPTY_STATE,
			   "LockStore::putState() : attempted write of empty state\n"));

	return FALSE;
    }

    if (getSegVaddr() != 0)
    {
	char buff[256];
	ostrstream temp(buff,256);
	long offset;

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	checkIfConsistent();
	
	if ((stateSize + sizeof(SlotHeader)) <= segZero.slotSize)
	{
	    if (slotNo > 0)
	    {
		SlotHeader b = redZone;
		offset = slotNo * segZero.slotSize;

		/* check that header is still intact */
		
		if ((get(offset, sizeof(SlotHeader), (char *)&b) == sizeof(SlotHeader)) &&
		    (b.guard == redZone.guard))
		{
		    b.stateSize = stateSize;
		    b.chkSum = checkSum(state.buffer(), stateSize);
		    
		    if (put(offset, sizeof(SlotHeader), (char *) &b) == sizeof(SlotHeader))
		    {
			if (put(offset + sizeof(SlotHeader), b.stateSize, state.buffer()) != b.stateSize)
			{
			    addError(new Error(FAC_OBJECT_STORE, OS_WRITE_FAILED,
					       "LockStore::putState() : failed to write into store\n"));
			    
			    return FALSE;
			}

			/* update control info */
			put(0, sizeof(segZero), (char *)&segZero);
			return TRUE;
		    }
		}
		else
		{
		    /* State header is corrupt */
		    temp << "LockSegment::putState() : corrupt state header in slot: " << slotNo
			 << " for segment: " << shmid()
			 << hex << " guard: " << b.guard << dec << " size: " << b.stateSize << ends;
		
		    addError(new Error(FAC_OBJECT_STORE, OS_CORRUPT, buff));
		}
	    }
	}
	else
	    addError(new Error(FAC_OBJECT_STORE, OS_TOO_BIG,
			       "LockSegment::putState() : state to large for slot\n"));
    }
    
    return FALSE;
}

/*
 * Allocate a new slot using the allocation map from slot 0.
 * Assume control information is valid and up to date.
 */

long LockSegment::allocateSlot ( )
{  
    if (isValid())
    {
	long slotNo = 1;		/* slot 0 is pre-allocated */
	
        /* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	checkIfConsistent();
	
	while ((slotNo < segZero.numSlots) &&
	       (segZero.allocMap[slotNo] == IS_ALLOC))
	    slotNo++;

	if (slotNo >= segZero.numSlots)
	    return -1;
	else
	{
	    segZero.allocMap[slotNo] = IS_ALLOC;
	    segZero.slotsInUse++;	

	    /* update control info */

	    put(0, sizeof(segZero), (char *)&segZero);
	    
	    return slotNo;
	}
    }

    return -1;
    
}

Boolean LockSegment::hasSpareSlots ()
{
    if (getSegVaddr() != 0)
    {
	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	checkIfConsistent();
	
	return segZero.slotsInUse < segZero.numSlots;
	    
    }

    return FALSE;
    
}

Boolean LockSegment::checkIfConsistent ()
{
    Boolean result = TRUE;
    int inUse = 0;

#ifdef DEBUG
    
    for (int i = 0; i < segZero.numSlots; i++)
    {
	if (segZero.allocMap[i] == IS_ALLOC)
	    inUse++;
    }
    
    if (inUse != segZero.slotsInUse)
    {
	char errBuff[256];
	ostrstream os(errBuff, 256);

	os << "consistency check failed for segment id " << shmid()
	   << " marked in use: " << inUse
	   << " thought in use: " << segZero.slotsInUse << ends;
	
	result = FALSE;
	addError(new Error(FAC_OBJECT_STORE, LS_BAD_SLOT, errBuff));
    }
    
#endif
    
    return result;
}
