/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockTable.h,v 1.1 1995/03/03 13:59:03 ngdp Exp $
 */

#ifndef LOCKTABLE_H_
#define LOCKTABLE_H_

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

#ifndef LTENTRY_H_
#  include "LTEntry.h"
#endif

// @NoRemote, @NoMarshall
class LockList;

// @NoRemote, @NoMarshall
class LockListChain;

const int DEFAULT_TABLE_SIZE = 11;

//  @NoWarnings, @NoRemote, @NoMarshall
class LockTable
{
public:
    /* Constructors and destructor */
    LockTable (int = DEFAULT_TABLE_SIZE);
    ~LockTable ();
    
    LockTableEntry *lookFor (const Uid&, const TypeName);
    
private:
    int hash (const Uid&, const TypeName) const;
    
    LockTableEntry **table;		/* pointer to array of Locators */
    int tableSize;
};

#endif
