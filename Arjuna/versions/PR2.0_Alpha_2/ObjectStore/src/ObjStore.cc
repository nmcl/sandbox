/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.cc,v 1.1 1993/11/03 14:36:34 nmcl Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTORELIST_H_
#  include "ObjStoreList.h"
#endif

static const char RCSid[] = "$Id: ObjStore.cc,v 1.1 1993/11/03 14:36:34 nmcl Exp $";


/*
 * This is the base class from which all object store types are derived.
 * Note that because object store instances are stateless, to improve
 * efficiency we try to only create one instance of each type per process.
 * Therefore, the create and destroy methods are used to instead of new
 * and delete. If an object store is accessed via create it *must* be
 * deleted using destroy. Of course it is still possible to make use of
 * new and delete directly and to create instances on the stack.
 */

ObjectStoreList* ObjectStore::headOfList = 0;


ObjectStore::ObjectStore ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::ObjectStore ()\n" << flush;
#endif
}

ObjectStore::~ObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::~ObjectStore ()\n" << flush;
#endif
}

void ObjectStore::addToList (ObjectStore* (*func1)(const char*), void (*func2)(const char*), const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void ObjectStore::addToList ( " << tn << " )" << endl;
#endif

    if (!headOfList)
	headOfList = new ObjectStoreList(func1, func2, tn);
    else
    {
	Boolean found = FALSE;
	ObjectStoreList *marker = headOfList, *trail = 0;
	
	while ((!found) && (marker))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	    debug_stream << "ObjectStore::addToList - Comparing " << tn << " with " << marker->type() << endl;
#endif
	    if (::strcmp(tn, marker->type()) != 0)
	    {
		trail = marker;
		marker = marker->next;
	    }
	    else
		found = TRUE;
	}
	
	if (!found)
	    trail->next = new ObjectStoreList(func1, func2, tn);
    }
}

void ObjectStore::destroy (ObjectStore*& toDelete)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void ObjectStore::destroy ( " << toDelete->type() << " )" << endl;
#endif

    if (headOfList)
    {
      ObjectStoreList* marker = headOfList;

      while (marker)
      {
	if (::strcmp(marker->type(), toDelete->type()) == 0)
	{
	    if (::strcmp(toDelete->type(), "RemotePersistentObjectStore") == 0)
		delete toDelete;
	    else
		marker->destroy(toDelete->getStoreName());

	    toDelete = 0;
	    return;
	}
	else
	  marker = marker->next;
      }
    }
}

ObjectStore* ObjectStore::create (const TypeName name, const char* locationOfStore)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore* ObjectStore::create ( " << name << " )" << endl;
#endif

    if (headOfList)
    {    
	ObjectStoreList* marker = headOfList;
    
	while (marker)
	{
	    if (::strcmp(marker->type(), name) == 0)
		return marker->create(locationOfStore);
	    else
		marker = marker->next;
	}
    }
    
    error_stream << FATAL << "Cannot find " << name << " object store."
		 << "\n#include RemoteOSTypes.h or LocalOSTypes.h" << endl;

    return 0;  // we will not get here, but this stops compiler warnings!
}

void ObjectStore::storeLocation (const char*) {}

void ObjectStore::useStoreLocation (const Boolean) {}
