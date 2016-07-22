/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockSeg.cc,v 1.10 1993/03/22 09:19:52 ngdp Exp $
 */

/*
 * Simple Lock Store Segment. Layered on top of SharedSegment it manages
 * a shared segment as an array of slots. Slot 0 is reserved for a table
 * that maps unique ids (Uid's) to slots. This ensures that all object
 * states with the same uid end up in the same slot.
 * Each slot contains an ObjectState preceded by 4 bytes that indicate
 * the size of the state.
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

#ifndef SHAREDSEG_H_
#  include "SharedSeg.h"
#endif

#ifndef LOCKSEG_H_
#  include "LockSeg.h"
#endif

static const char RCSid[] = "$Id: LockSeg.cc,v 1.10 1993/03/22 09:19:52 ngdp Exp $";

/* 
 * Managing shared memory dynamically is hard particularly the table in 
 * slot 0 which grows and shrinks. Treat it as a fixed size array instead
 * for simplicity.
 */

static const int MAX_SEGMENT_SIZE = 512 * 1024;
static const int MAX_SLOTS = 512;
static const char IS_ALLOC = 1;
static const char IS_FREE = 0;

/* The structure that sits in slot 0 */

static struct SegmentZero
{
    size_t slotSize;			/* size of a single slot */
    long numSlots;			/* number in region */
    long slotsInUse;			/* number actually in use */
    char allocMap[MAX_SLOTS];		/* allocation map */
} segZero;

/*
 * Public constructors and destructor 
 */

/*
 * Lock segment constructor. Since this looks like a shared segment it has
 * two parameters BUT we ignore the key_t parameter here as its not needed
 */

LockSegment::LockSegment ( key_t, size_t slotSize )
                         : SharedSegment()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::LockSegment("
	         << slotSize << ")\n" << flush;
#endif
    
    /* 
     * Slot 0 contains the control info (allocation map
     * and header). Ensure the requested slot size will hold it. If not
     * ignore the requested size and use our own.
     */

    size_t minSize = sizeof(struct SegmentZero);
    size_t allocSize = (slotSize > minSize ? slotSize : minSize);
    long slotCount = MAX_SEGMENT_SIZE / allocSize;
    

    if (slotCount > MAX_SLOTS)
	slotCount = MAX_SLOTS;
    
    /* Note that new segments are already inialised to 0 */

    if (smInit(IPC_PRIVATE, allocSize*slotCount) != 0)
    {
	/* Initialise segment 0 */

	segZero.slotSize = allocSize;
	segZero.numSlots = slotCount;
	segZero.slotsInUse = 1;

	/* slot 0 must be marked as allocated */

	segZero.allocMap[0] = IS_ALLOC;

	put(0, sizeof(segZero), (const char *)&segZero);
    }
    
    if (!isValid())
	error_stream << WARNING 
		     << "LockSegment::LockSegment failed unexpectedly\n" << flush;
    
}

/*
 * Create new LockSegment manager for existing segment
 */

LockSegment::LockSegment ( int segId )
                         : SharedSegment(segId)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::LockSegment(" << segId
	         << ")\n" << flush;
#endif

    if (!isValid())
	error_stream << WARNING 
		     << "LockSegment::LockSegment failed to attach to existing segment\n" 
		     << flush;
}

/*
 * Destructor - does nothing except print debugging information if requested
 */

LockSegment::~LockSegment ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::~LockSegment()\n" << flush;
#endif
}

/*
 * Public non-virtual functions
 */

Boolean LockSegment::getState ( const Uid& u,
				const TypeName tName,
				ObjectState *&state,
				long slotNo ) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream << "LockSegment::getState(" << u << ", " 
	         << tName << ")\n" << flush;
#endif

    state = 0;
    
    if (isValid())
    {
	size_t stateSize;

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);

	if (slotNo >= 0)
	{
	    long offset = slotNo * segZero.slotSize;

	    /* Get size of saved state */

	    if (get(offset, sizeof(stateSize), (char *)&stateSize) == sizeof(stateSize))
	    {
		if (stateSize > 0)
		{
		    state = new ObjectState(u, tName, stateSize,
					    new char[stateSize]);

		    if (get(offset+sizeof(stateSize), stateSize,
			    state->buffer()) != stateSize)
		    {
			/* State itself appears corrupt */

			delete state;
			state = 0;
			error_stream << WARNING 
				     << "LockSegment::getState() : corrupt state buffer!\n"
				     << flush;
		    }
		    return TRUE;
		} 
		else if (stateSize == 0)
		{
		    return TRUE;
		}
	    }
	    else
	    {
		/* State header is corrupt */

		error_stream << WARNING
			     << "LockSegment::getState() : corrupt state header!\n"
			     << flush;
	    
	    }
	}
	else
	    error_stream << WARNING
			 << "LockSegment::getState() : invalid slot number " << slotNo
			 << "\n" << flush;

    }
    else
	error_stream << WARNING
		     << "LockSegment::getState() : shared segment id " << shmid()
		     << " is marked invalid\n" << flush;
    
    
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
    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream <<"LockSegment::freeSlot(" << slotNo << ")\n" << flush;
#endif

    if (getSegVaddr() != 0)
    {

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);

	if (slotNo > 0)
	{
	    long offset =  slotNo * segZero.slotSize;
	    const size_t zero = 0;

	    put(offset, sizeof(zero), (char *)&zero);
	    segZero.allocMap[slotNo] = IS_FREE;
	    segZero.slotsInUse--;
	    
	    put(0, sizeof(segZero), (char *)&segZero);
	}
	else
	    error_stream << WARNING
			 << "LockSegment::freeSlot() attempting to free slot " << slotNo
			 << endl << flush;
	return TRUE;
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
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_SHMEM << VIS_PUBLIC;
    debug_stream <<"LockSegment::putState("  << slotNo << ", "
	         << (void *)&state << ")\n" << flush;
#endif

    size_t stateSize = state.size();

    if (stateSize == 0)
    {
	error_stream << WARNING 
	             << "LockSegment::putState() : attempted write of empty state\n" 
		     << flush;
	return FALSE;
    }

    if (getSegVaddr() != 0)
    {
	long offset;

	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	
	if (stateSize <= segZero.slotSize)
	{
	    if (slotNo >= 0)
	    {
		offset = slotNo * segZero.slotSize;

		if (put(offset, sizeof(stateSize), (char *) &stateSize)
		    == sizeof(stateSize))
		{
		    if (put(offset + sizeof(stateSize), stateSize,
			    state.buffer()) != stateSize)
		    {
			error_stream << WARNING 
				     << "LockSegment::putState() : failed to write into store\n"
				     << flush;
			return FALSE;
		    }

		    /* update control info */
		    put(0, sizeof(segZero), (char *)&segZero);
		    return TRUE;
		}
	    }
	}
	else
	    error_stream << WARNING 
			 << "LockSegment::putState() : state to large for slot\n" 
			 << flush;
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
    else
	    error_stream << WARNING 
			 << "LockSegment::allocateSlot() shared segment " << shmid()
			 << " is marked invalid\n" << flush;
    return -1;
    
}

Boolean LockSegment::hasSpareSlots ()
{
    if (getSegVaddr() != 0)
    {
	/* read control info */
	
	get(0, sizeof(segZero), (char *)&segZero);
	
	return segZero.slotsInUse < segZero.numSlots;
	    
    }

    return FALSE;
    
}
