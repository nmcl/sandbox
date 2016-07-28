/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LTEntry.cc,v 1.2 1995/03/06 10:23:36 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef LTEntry_H_
#  include "LTEntry.h"
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

static const char RCSid[] = "$Id: LTEntry.cc,v 1.2 1995/03/06 10:23:36 ngdp Exp $";

/*
 * Public constructor and destructor
 * Don't allow object Type to be the null string
 */

LockTableEntry::LockTableEntry ( const Uid& objUid, const TypeName name )
			       : heldLocks(new LockList),
				 next(0),
				 objectType(name == 0 ? clone("") : clone(name)),
				 objectUid(objUid)
{
}

LockTableEntry::~LockTableEntry ()
{
    if (heldLocks)
	delete heldLocks;
    
    if (next)
    {
	delete next;
    }
    
    if (objectType)
#ifndef GCC_DELETE_BUG
	delete [] objectType;
#else
        delete  objectType;
#endif    
}

LockList *LockTableEntry::getLockList ()
{
    return heldLocks;
}

LockTableEntry *LockTableEntry::lookFor ( const Uid& objUid,
					  const TypeName type )
{
    LockTableEntry *rVal = 0;
    char *p = (type == 0 ? "" : type);
    
    if ((objUid == objectUid) && (strcmp(p, objectType) == 0))
    {
	rVal = this;
    }
    else
	if (next)
	    rVal = next->lookFor(objUid, type);
	else
	{
	    rVal = next = new LockTableEntry(objUid, type);
	}
    
    return rVal;
}

Boolean LockTableEntry::replaceList ( LockList *newList )
{
    if (heldLocks)
	delete heldLocks;

    heldLocks = newList;

    return TRUE;
}

