/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockSeg.h,v 1.4 1995/10/02 15:14:14 ngdp Exp $
 */

#ifndef LOCKSEG_H_
#define LOCKSEG_H_

/*
 *
 * Lock Segment class. Mangages a chunk of shared memory as a collection
 * of fixed sized slots. Slot 0 is reserved for control information.
 * There is a maximum of MAX_SLOTS-1 slots available for use (fixed 
 * at compile time).
 *
 */

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#if defined(HAVE_MMAP) && defined(USE_MMAP)
#  ifndef MAPPEDSEG_H_
#    include "MappedSeg.h"
#  endif
#  define BaseSegment MappedSegment
#else
#  ifndef SHAREDSEG_H_
#    include "SharedSeg.h"
#  endif
#  define BaseSegment SharedSegment
#endif

class ObjectState;
class Uid;

class Lock_Segment : public BaseSegment
{
public:
    /* Constructors and destructor */

    Lock_Segment (key_t key, size_t slotSize);
#ifndef WIN32
    Lock_Segment (int segId);
#else
	Lock_Segment (key_t segId);
#endif

    ~Lock_Segment ();

    /* non-virtual member functions */

    long allocateSlot ();
    Boolean hasSpareSlots ();

    Boolean getState (const Uid&, const TypeName, ObjectState*&, long);
    Boolean putState (long slotNo, const ObjectState& state);
    Boolean freeSlot (long slotNo);

private:
    /* non-virtual member functions */
    Boolean checkIfConsistent ();
};

#endif
