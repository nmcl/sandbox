/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreList.cc,v 
 */

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef STORELIST_H_
#  include <ObjectStore/StoreList.h>
#endif

StoreList::StoreList ()
                      : useCount(0),
			instance(0),
			next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "StoreList::StoreList ()" << endl;
#endif    
}

StoreList::~StoreList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;    
    debug_stream << "StoreList::~StoreList ()" << endl;
#endif

    if (instance)
	delete instance;
}
