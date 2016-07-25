/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockSeg.h,v 1.1 1993/11/03 12:29:01 nmcl Exp $
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

#ifndef SHAREDSEG_H_
#  include "SharedSeg.h"
#endif

class ObjectState;
class Uid;

class LockSegment : public SharedSegment
{
public:
    /* Constructors and destructor */
    
    LockSegment (key_t key, size_t slotSize);
    LockSegment (int segId);
    ~LockSegment ();

    /* non-virtual member functions */

    long allocateSlot ();
    Boolean hasSpareSlots ();

    Boolean getState (const Uid&, const TypeName, ObjectState*&, long) const;
    Boolean putState (long slotNo, const ObjectState& state);
    Boolean freeSlot (long slotNo);

private:
    /* non-virtual member functions */

};

#endif
