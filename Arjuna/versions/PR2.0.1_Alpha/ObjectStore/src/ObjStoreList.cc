/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreList.cc,v 
 */


#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTORELIST_H_
#  include "ObjStoreList.h"
#endif


ObjectStoreList::ObjectStoreList (ObjectStore* (*func1)(const char*),
				  void (*func2)(const char*), const TypeName tn)
                                                                               : StoreCreate(func1),
										 StoreDestroy(func2),
										 next(0),
										 typeName(tn)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStoreList::ObjectStoreList ( " << typeName << " )" << endl;
#endif
}

ObjectStoreList::~ObjectStoreList ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStoreList::~ObjectStoreList ()" << endl;
#endif

    if (next)
	delete next;
}

const TypeName ObjectStoreList::type () const { return typeName; }

ObjectStore* ObjectStoreList::create (const char* locationOfStore) { return (*StoreCreate)(locationOfStore); }

void ObjectStoreList::destroy (const char* locationOfStore) { (*StoreDestroy)(locationOfStore); }
