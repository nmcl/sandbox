/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStore.h,v 1.10 1993/06/11 13:05:07 nmcl Exp $
 */

#ifndef LOCKSTORE_H_
#define LOCKSTORE_H_

/*
 *
 * Simple Shared Lock Store. Provides similar interface as the object store
 * but uses shared memory. Currently very simple.
 *
 */

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
class LockSegment;
class Uid;
struct MappingTable;  // xlC bug or ANSI bug??

static const int SEGMENT_LIMIT = 6;		/* max number of segments per type */

class LockStore : public SharedSegment
{
public:
    /* Constructors and destructor */
    
    LockStore (const key_t key);
    ~LockStore ();
    
    /* non-virtual member functions */
    
    Boolean read_state (const Uid& u, const TypeName tName, ObjectState*&);
    Boolean remove_state (const Uid& u, const TypeName tName);
    Boolean write_committed (const Uid& u, const TypeName tName, const ObjectState& state);
    
private:
    long allocateSegment (size_t slotSize);
    LockSegment *attachSegment (long segIndex);
    long lookfor (const Uid&) const;
    long insert (const Uid&);
    
    LockSegment *lockSegs[SEGMENT_LIMIT];
    key_t lockSegKey;
    MappingTable *mapTablePtr;
};

#endif
