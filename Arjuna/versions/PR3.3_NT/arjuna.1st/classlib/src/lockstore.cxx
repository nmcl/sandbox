/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStore.cc,v 1.10 1995/10/02 15:13:27 ngdp Exp $
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

static const char RCSid[] = "$Id: LockStore.cc,v 1.10 1995/10/02 15:13:27 ngdp Exp $";

static const int OBJECT_LIMIT = 1024;	        /* Max number of objects of a type */
static const size_t MIN_STATE_SIZE = 1024;	/* min size of a segment slot */
static const int UID_NAME_LEN = 48;		/* Uid as string no longer than this */
static const int NULL_MAP_INDEX = -1;
static const int NULL_SLOT_NO = -1;
static const int NULL_SEGMENT_NO = -1;

#ifndef WIN32
static const int NULL_SHMID = -1;
#else
static char NULL_SHMID = '\0';
#endif

struct UidMap
{
    long slotNo;			/* Slot Uid is mapped to */
    long segmentNo;			/* Segment containing slot */
    char uidString[UID_NAME_LEN];	/* Uid as a string */
};

struct SegmentDescriptor
{
#ifndef WIN32
    int smId;					/* Shared memory id of segment */
#else
	char smId[MAX_PATH];
#endif
    size_t slotSize;			/* size of a single slot */
};

struct MappingTable
{
    int segmentInitialised;
    int mapEntries;
    UidMap mapTable[OBJECT_LIMIT];		/* uid to segment & slot map */
    SegmentDescriptor segmentTable[SEGMENT_LIMIT];
};

/* The null map table entry template */

static UidMap nullMap = {NULL_SLOT_NO, NULL_SEGMENT_NO, ""};

static SegmentDescriptor nullSeg = {NULL_SHMID, 0};

/*
 * Public constructors and destructor 
 */

/*
 * Create the lock store. 
 * This creates or initialises the control segment but the actual
 * Lock_Segments are created lazily as needed. If control segment creation
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
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::LockStore(" << key << ")" << endl;
    debug_stream.unlock();
    
#endif

    if (isValid())
    {
		int i;

		mapTablePtr = (struct MappingTable *)getSegVaddr();
		
		if (mapTablePtr->segmentInitialised)
		{
			for (i = 0; i < SEGMENT_LIMIT; i++)
			{
#ifndef WIN32
				if (mapTablePtr->segmentTable[i].smId != NULL_SHMID)
#else
				if (mapTablePtr->segmentTable[i].smId[0] != NULL_SHMID)
#endif
					lockSegs[i] = new Lock_Segment(mapTablePtr->segmentTable[i].smId);
				else
					lockSegs[i] = 0;
			}
		}
		else
		{
			/*
			 * initialise the null map entry here to ensure NIL_UID
			 * has been created
			 */

			ostrstream conv(nullMap.uidString, UID_NAME_LEN, ios::out);

			nullMap.slotNo = NULL_SLOT_NO;
			nullMap.segmentNo = NULL_SEGMENT_NO;
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
			
			mapTablePtr->mapEntries = 0;
			mapTablePtr->segmentInitialised = 1;
		}
    }
    else
    {
		addError(new Error(FAC_OBJECT_STORE, SHM_INVALID,
				"LockStore::LockStore() failed to allocate shared memory for lock storage - (increase system limits?)\n"));
    }
}

LockStore::~LockStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::~LockStore()" << endl;
    debug_stream.unlock();
    
#endif

    if (isValid())
    {
		for (int i = 0; i < SEGMENT_LIMIT; i++)
		{
			if (lockSegs[i] != 0)
			{
				/* Am I the last user? */
				
				if (lockSegs[i]->usage() == 1)
				{
					mapTablePtr->segmentTable[i] = nullSeg;

					/*
					 * Find entries in object mapping table with matching segid
					 * and null out slot no also
					 */
					
					for (int j = 0; j < OBJECT_LIMIT; j++)
					{
						if (mapTablePtr->mapTable[j].segmentNo == i)
							mapTablePtr->mapTable[j].slotNo = NULL_SLOT_NO;
					}
					
				}
				
				delete lockSegs[i];
					lockSegs[i] = 0;
			}
		}
    }
}

/*
 * Public non-virtual functions
 */

/*
 * Emulate the read_state function of ObjectStore. Extracts an instance
 * of ObjectState out of the shared memory region which is managed as an 
 * instance of Lock_Segment and returns it to the user. 
 */

Boolean LockStore::read_state ( const Uid& u,
								const TypeName tName,
								ObjectState *& state)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LockStore::read_state(" << u << ", "
	         << tName << ")" << endl;
    debug_stream.unlock();
    
#endif

    state = 0;
    Error *error = 0;
    char buff[256];
    ostrstream temp(buff, 256);

    if (isValid())
    {
		long mapIndex = lookfor(u);

		if (mapIndex != NULL_MAP_INDEX)
		{
			long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;
			long slotNo = mapTablePtr->mapTable[mapIndex].slotNo;

			if ((segmentIndex != NULL_SEGMENT_NO) && (slotNo != NULL_SLOT_NO))
			{
				Lock_Segment *useSeg = attachSegment(segmentIndex);
				
				if (useSeg != 0)
					return useSeg->getState(u, tName, state, slotNo);
				else
				{
					temp << "LockStore::read_state() attach for segment index "
					 << segmentIndex << " failed" << endl << ends;

					error = new Error(FAC_OBJECT_STORE, SHM_ATTACH_FAILED, buff);

					addError(error);

					return FALSE;
				}
			} 
		}
	
		return TRUE;
    } 
    else
    {
		temp << "LockStore::read_state() shared segment " << shmid()
			 << "is marked invalid" << endl << ends;
    
		error = new Error(FAC_SHMEM, SHM_INVALID, buff);

		addError(error);
    }

    return FALSE;    
}

Boolean LockStore::remove_state ( const Uid& u, const TypeName tname )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream <<"LockStore::remove(" << u << ", " << tname << ")" << endl;
    debug_stream.unlock();
    
#endif

    Error *error = 0;
    char buff[256];
    ostrstream temp(buff, 256);

    if (isValid())
    {
		long mapIndex = lookfor(u);

		if (mapIndex != NULL_MAP_INDEX)
		{
			long slotIndex = mapTablePtr->mapTable[mapIndex].slotNo;
			
			mapTablePtr->mapTable[mapIndex].slotNo = NULL_SLOT_NO;
			
			if (slotIndex != NULL_SLOT_NO)
			{
				long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;	
				Lock_Segment *useSeg = attachSegment(segmentIndex);
				
				if (useSeg != 0)
					return useSeg->freeSlot(slotIndex);
				else
				{
					temp << "LockStore::remove_state() segment attach for segment "
					 << segmentIndex << " failed" << endl << ends;

					error = new Error(FAC_SHMEM, SHM_ATTACH_FAILED, buff);

					addError(error);

					return FALSE;
				}
			}
			else
				return TRUE;
		}
		return TRUE;
    }
    else
    {
		temp << "LockStore::remove_state() shared segment " << shmid()
			 << "is marked invalid" << endl << ends;
    
		error = new Error(FAC_OBJECT_STORE, SHM_INVALID, buff);

		addError(error);
    }

    return FALSE;    
}

Boolean LockStore::write_committed ( const Uid& u,
									 const TypeName tName,
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
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream <<"LockStore::write_committed("  << u << ", "
	         << (void *)&state << ")" << endl;
    debug_stream.unlock();
    
#endif

    char buff[512];
    ostrstream temp(buff, 512);
    Error *error = 0;
    size_t stateSize = state.size();
    
    if (stateSize <= 0)
    {
		error = new Error(FAC_OBJECT_STORE, OS_EMPTY_STATE,
				  "LockStore::write_state() : attempted write of empty state!\n");
		addError(error);
		
		return FALSE;
    }
    
    if (isValid())
    {
		long mapIndex = insert(u);
		
		if (mapIndex != NULL_MAP_INDEX)
		{
			long segmentIndex = mapTablePtr->mapTable[mapIndex].segmentNo;
			long slotIndex = mapTablePtr->mapTable[mapIndex].slotNo;
			Lock_Segment *useSeg = 0;
			
			if (segmentIndex != NULL_SEGMENT_NO)
			{
				/* A segment should exist */

				useSeg = attachSegment(segmentIndex);
				
				if ((useSeg == 0) ||
					(!useSeg->hasSpareSlots()) ||
					(stateSize > mapTablePtr->segmentTable[segmentIndex].slotSize))
				{
					/* But its slots are now too small or full */
					
					if ((useSeg != 0) && (slotIndex != NULL_SLOT_NO))
					{
						useSeg->freeSlot(slotIndex);
						mapTablePtr->mapTable[mapIndex].slotNo = NULL_SLOT_NO;
						mapTablePtr->mapTable[mapIndex].segmentNo = NULL_SEGMENT_NO;
					}
					segmentIndex = allocateSegment(stateSize);
				}
			} else
				segmentIndex = allocateSegment(stateSize);
			
			if (segmentIndex != NULL_SEGMENT_NO)
			{
				mapTablePtr->mapTable[mapIndex].segmentNo = segmentIndex;
				useSeg = attachSegment(segmentIndex);
				
				if (useSeg != 0)
				{
					if ((slotIndex != NULL_SLOT_NO) ||
					((slotIndex = useSeg->allocateSlot()) != NULL_SLOT_NO))
					{
						mapTablePtr->mapTable[mapIndex].slotNo = slotIndex;
						
						if (!(useSeg->putState(slotIndex, state)))
						{
							error = new Error(FAC_OBJECT_STORE, OS_WRITE_FAILED,
									  "LockStore::write_state() : failed to write into lock store\n");

							addError(error);
							return FALSE;
						}
					
						return TRUE;
					}
					else
						error = new Error(FAC_OBJECT_STORE, LS_BAD_SLOT,
								"LockStore::write_state() failed to allocate slot\n");
			}
			else
			{
				temp << "LockStore::write_state() failed to attach segment: "
				 << mapTablePtr->mapTable[mapIndex].segmentNo << " slot: "
				 << mapTablePtr->mapTable[mapIndex].slotNo << " uid: "
				 << mapTablePtr->mapTable[mapIndex].uidString
				 << endl << ends;
				error = new Error(FAC_OBJECT_STORE, SHM_ATTACH_FAILED, buff);
			}
			}
			else
			{
				temp << "LockStore::write_state() failed to allocate segment for uid: "
					 << mapTablePtr->mapTable[mapIndex].uidString
					 << endl << ends;
				error = new Error(FAC_OBJECT_STORE, LS_SEGMENT_ALLOC_FAILED, buff);
			}
		}
		else
		{
			temp << "LocalStore::write_state() map insert failed for object "
			 << u << " of type " << tName << endl << ends;
			error = new Error(FAC_OBJECT_STORE, LS_MAP_ALLOC_FAILED, buff);
		}
    }
    else
    {
		temp << "LocalStore::write_state() segment with id " 
			 << shmid() << " is marked invalid" << endl << ends;
		error = new Error(FAC_OBJECT_STORE, SHM_INVALID, buff);
    }

    if (error)
		addError(error);
    return FALSE;
}

/*
 * Table look up has determined which Lock_Segment to use. Attach it if needed
 */

Lock_Segment *LockStore::attachSegment ( long segIndex )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LockStore::attachSegment(" << segIndex << ")" << endl;
    debug_stream.unlock();
    
#endif

    /* Determine if another process has created a segment for use */

#ifndef WIN32
    if ((lockSegs[segIndex] == 0) && (mapTablePtr->segmentTable[segIndex].smId != NULL_SHMID))
#else
    if ((lockSegs[segIndex] == 0) && (mapTablePtr->segmentTable[segIndex].smId[0] != NULL_SHMID))
#endif
	{
		Lock_Segment *t = new Lock_Segment(mapTablePtr->segmentTable[segIndex].smId);

		if (t)
		{
			if (t->isValid())
				lockSegs[segIndex] = t;
			else
			{
				ErrorList& eList = t->getErrorList();
				Error *error;

				while ((error = eList.pop()))
					addError(error);
				delete t;
			}
		}
    }

    return lockSegs[segIndex];
}

long LockStore::allocateSegment ( size_t slotSize )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LockStore::allocateSegment(" << slotSize << ")" << endl;
    debug_stream.unlock();
    
#endif

    Lock_Segment *newSeg = 0;
    long i = 0;

    for (i = 0; i < SEGMENT_LIMIT; i++)
    {
#ifndef WIN32
		if ((mapTablePtr->segmentTable[i].smId != NULL_SHMID) &&
			(mapTablePtr->segmentTable[i].slotSize >= slotSize))
#else
		if ((mapTablePtr->segmentTable[i].smId[0] != NULL_SHMID) &&
			(mapTablePtr->segmentTable[i].slotSize >= slotSize))
#endif
		{
			newSeg = attachSegment(i);
			if ((newSeg) && (newSeg->hasSpareSlots()))
				return i;
		}
    }

    /* No existing segment is usable can we create one? */

    for (i = 0; i < SEGMENT_LIMIT; i++)
    {
#ifndef WIN32
		if (mapTablePtr->segmentTable[i].smId == NULL_SHMID )
#else
		if (mapTablePtr->segmentTable[i].smId[0] == NULL_SHMID )
#endif
		{
			size_t roundedSize = 2048;

			while (roundedSize <= slotSize)
				roundedSize <<= 1;
		
			newSeg = new Lock_Segment(lockSegKey, roundedSize);

			if (newSeg->isValid())
			{
				lockSegs[i] = newSeg;
#ifndef WIN32
				mapTablePtr->segmentTable[i].smId = newSeg->shmid();
#else
				::strcpy(mapTablePtr->segmentTable[i].smId, newSeg->shmid());
#endif
				mapTablePtr->segmentTable[i].slotSize = roundedSize;
				return i;
			}
			else
			{
				/* Segment creation failed */
				
				delete newSeg;	/* it was invalid */
			}
		}
    }

    return NULL_SEGMENT_NO;
}

/*
 * Map uid to map table slot number
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

    return NULL_MAP_INDEX;
}

/*
 * Insert uid into mapping table. Table is held sorted so that binary
 * search can be used in lookup routine.
 */

long LockStore::insert ( const Uid& u ) 
{
    long first = 0;
    long last = 0;
    long ind = 0;
    int res = 0;

    char uidBuff[UID_NAME_LEN];
    ostrstream conv(uidBuff, UID_NAME_LEN, ios::out);
    
    conv << u << ends;	
    
    if (mapTablePtr->mapEntries >= OBJECT_LIMIT)
		compactMap ();

    last = mapTablePtr->mapEntries;
    
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
    
    if (mapTablePtr->mapEntries < OBJECT_LIMIT)
    {
		ind = mapTablePtr->mapEntries;
		while (ind > first)
		{
			mapTablePtr->mapTable[ind] = mapTablePtr->mapTable[ind-1];
			ind--;
		}

		::strcpy(mapTablePtr->mapTable[first].uidString,uidBuff);
		mapTablePtr->mapTable[first].segmentNo = NULL_SEGMENT_NO;
		mapTablePtr->mapTable[first].slotNo = NULL_SLOT_NO;
		mapTablePtr->mapEntries++;
    }
    else
		first = NULL_MAP_INDEX;
    
    return first;
}

void LockStore::compactMap ()
{
    Boolean compacted = FALSE;
    long first = 0;
    long last = mapTablePtr->mapEntries;
    long temp1 = 0;
    long temp2 = 0;
    long step = 1;

    while (!compacted)
    {
		/* scan for discarded slot */

		while ((first < last) &&
			   (mapTablePtr->mapTable[first].slotNo != NULL_SLOT_NO))
			first++;

		if (first == last)
		{
			compacted = TRUE;
			break;
		}

		temp1 = first;

		/* Shuffle higher slots down - attempt to compact several contiguous slots at a time */

		temp2 = temp1+1;
		
		while ((temp2 < last) &&
			   (mapTablePtr->mapTable[temp2].slotNo == NULL_SLOT_NO))
		{
			temp2++;
		}

		step = temp2-temp1;
		
		/* check entire tail is not discarded */
		
		while (temp1+step < last)
		{
			mapTablePtr->mapTable[temp1] = mapTablePtr->mapTable[temp1+step];
			temp1++;
		}
		
		mapTablePtr->mapEntries -= step;
		last -=step;
    }

    for (int i = mapTablePtr->mapEntries; i < OBJECT_LIMIT; i++)
		mapTablePtr->mapTable[i] = nullMap;
}
