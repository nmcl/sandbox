/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockTable.cc,v 1.1 1995/03/03 13:59:12 ngdp Exp $
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

#ifndef LTENTRY_H_
#  include "LTEntry.h"
#endif

#ifndef LOCKTABLE_H_
#  include "LockTable.h"
#endif

static const char RCSid[] = "$Id: LockTable.cc,v 1.1 1995/03/03 13:59:12 ngdp Exp $";
     
/*
 * Public constructor and destructor
 */

LockTable::LockTable ( int initialsize )
		     : table(0),
		       tableSize(initialsize)
{
    table = new LockTableEntry *[tableSize];

    for (int i = 0; i < tableSize; i++)
    {
	table[i] = 0;
    }
}

LockTable::~LockTable ()
{
    if (table)
    {
	for (int i = 0; i < tableSize; i++)
	{
	    if (table[i])
		delete table[i];
	}
	delete [] table;
    }
}

LockTableEntry *LockTable::lookFor ( const Uid& objectUid,
				     const TypeName type )
{
    if (table)
    {
	int tableIndex = hash(objectUid, type);
	
	if (table[tableIndex] == 0)
	{
	    table[tableIndex] = new LockTableEntry(objectUid, type);
	}

	if (table[tableIndex] != 0)
	{
	    return table[tableIndex]->lookFor(objectUid, type);
	}
    }
    
    return 0;
}

int LockTable::hash ( const Uid& objectUid,
		      const TypeName name ) const
{
    char buffer[1024];
    ostrstream convBuffer(buffer, 1024);

    convBuffer << objectUid << name << ends;
    
    return checkSum(buffer, strlen(buffer)) % tableSize;
}

