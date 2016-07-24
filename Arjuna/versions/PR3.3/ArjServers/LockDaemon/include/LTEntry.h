/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LTEntry.h,v 1.1 1995/03/03 13:58:57 ngdp Exp $
 */

#ifndef LTENTRY_H_
#define LTENTRY_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * This class helps locate lists of locks held for the object
 * with the given Uid and TypeName
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @NoMarshall
class LockList;

//  @NoWarnings, @NoRemote, @NoMarshall
class LockTableEntry
{
public:
    /* Constructors and destructor */
    LockTableEntry (const Uid&, const TypeName);
    ~LockTableEntry ();

    LockList *getLockList ();
    LockTableEntry *lookFor (const Uid&, const TypeName);
    Boolean replaceList (LockList *);
    
private: 
    LockList *heldLocks;
    LockTableEntry *next;

    TypeName objectType;
    Uid objectUid;
};

#endif
