/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.cc,v 1.28 1995/10/09 08:28:00 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif
 
#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef STORELIST_H_
#  include "StoreList.h"
#endif

#ifdef WIN32
#include <windows.h>

#define MAXPATHLEN MAX_PATH
#endif

static const char RCSid[] = "$Id: ObjStore.cc,v 1.28 1995/10/09 08:28:00 ngdp Exp $";

const char* ObjectStore::ObjectStoreRoot = (char*) 0;

SetUpInfo::SetUpInfo ()
					  : _setUp(0),
					    _alwaysCreate(0),
						_headOfList(0),
						_new(0),
						_tn(0),
						_next(0)
{
}

SetUpInfo::~SetUpInfo ()
{
}

/*
 * STATIC helper functions for ObjectStore management.
 * These are static so that they are available at all time during execution
 * even during static object creation and destruction
 */

ObjectStore* create ( SetUpInfo& setupInf, const char* location )
{
    StoreList* slptr = 0;
    Boolean found = FALSE, endOfList = FALSE;

    if (location == 0)
		location = "";
    
    slptr = setupInf._headOfList;

    while ((slptr) && (!found) && (!endOfList))
    {
		if (!setupInf._alwaysCreate)
		{
			const char *sName = slptr->instance->getStoreName();
			
			if (sName == 0)
				sName = "";

			if (::strcmp(sName, location) == 0)
			{
				found = TRUE;
				break;
			}
		}
		
		if (slptr->next != 0)
			slptr = slptr->next;
		else
			endOfList = TRUE;
    }

    if (!found)
    {
		StoreList* toAdd = new StoreList;
		toAdd->instance = (*setupInf._new)(location);

		if (setupInf._headOfList == 0)
			setupInf._headOfList = toAdd;
		else
			slptr->next = toAdd;

		slptr = toAdd;
    }
    
    slptr->useCount++;
    return slptr->instance;
}

Boolean reference ( SetUpInfo& setupInf, ObjectStore* toRef )
{
    StoreList* slptr = (StoreList*) 0;
    Boolean found = FALSE, endOfList = FALSE;

    slptr = setupInf._headOfList;
    while ((slptr) && (!found) && (!endOfList))
    {
		if (slptr->instance == toRef)
			found = TRUE;
		else
		{
			if (slptr->next != 0)
				slptr = slptr->next;
			else
				endOfList = TRUE;
		}
    }
    
    if (!found)
    {
        error_stream << WARNING << "ObjectStoreSetUp::reference - cannot find objectstore instance." << endl;
		return FALSE;
    }
    
    slptr->useCount++;
    return TRUE;
}

void destroy ( SetUpInfo& setupInf, const char* location,
			   ObjectStore*& toDelete )
{
    if (toDelete == 0)
		return;
    
    StoreList *slptr = 0, *slmarker = 0;
    Boolean found = FALSE;
    
    if (location == 0)
		location = "";

    if (setupInf._headOfList == 0)
    {
		error_stream << WARNING 
				 << "Attempt to destroy object store instance without call on ObjectStore::create" 
				 << endl;

		return;
    }
    
    slptr = setupInf._headOfList;
    while ((slptr) && (!found))
    {
		if (slptr->instance == toDelete)
			found = TRUE;
		else
		{
			slmarker = slptr;
			slptr = slptr->next;
		}
    }
    
    if (!found)
    {
		error_stream << WARNING 
				 << "Attempt to destroy non-existant object store instance" 
				 << endl;
		delete toDelete;
		toDelete = 0;
		return;
    }
    
    slptr->useCount--;
    
    if (slptr->useCount == 0)
    {
		if (slmarker == 0)
			setupInf._headOfList = slptr->next;
		else
			slmarker->next = slptr->next;
		
		delete slptr;
		toDelete = 0;
    }
}

ObjectStoreSetUp::ObjectStoreSetUp ( SetUpInfo* info )
									: setupInf(info)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStoreSetUp::ObjectStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif

#ifndef RUNTIME_STATIC_INIT
    if (!setupInf->_setUp)
    {
		ObjectStore::addToList(setupInf);
		setupInf->_setUp = TRUE;
    }
#endif
}

ObjectStoreSetUp::~ObjectStoreSetUp () {}

/*
 * This is the base class from which all object store types are derived.
 * Note that because object store instances are stateless, to improve
 * efficiency we try to only create one instance of each type per process.
 * Therefore, the create and destroy methods are used instead of new
 * and delete. If an object store is accessed via create it *must* be
 * deleted using destroy. Of course it is still possible to make use of
 * new and delete directly and to create instances on the stack.
 */

SetUpInfo* ObjectStore::headOfList =  0;

ObjectStore::~ObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::~ObjectStore ()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Generate the full path of the store. location is always a relative name
 * NOTE this path always ends in a /
 */

char* ObjectStore::locateStore ( const char* location ) const
{
    char *objectStoreRoot = ::getenv("LOCALOSROOT");
    char *finalLocation = new char[MAXPATHLEN+1];
    int len = 0;
    
#ifdef WIN32
	char* OBJECTSTOREDIR_Q = ::getenv("OBJECTSTORE_LOCATION");

	if (!OBJECTSTOREDIR_Q)
	{
		/*
		error_stream << FATAL << "OBJECTSTORE_LOCATION not set." << endl;
		ArjunaFatal();
		*/
		OBJECTSTOREDIR_Q = "C:/temp";
	}
#endif

    ::strcpy(finalLocation, OBJECTSTOREDIR_Q);

    len = ::strlen(finalLocation);
    if ((len > 0) && (finalLocation[len-1] != '/'))
    {
		finalLocation[len++] = '/';
		finalLocation[len] = '\0';
    }
	
    ::strcat(finalLocation, type());
    
    if ((location == 0) || (::strlen(location) == 0))
    {
		if (objectStoreRoot && (::strlen(objectStoreRoot) > 0))
			location = objectStoreRoot;
		else
			location = "defaultStore/";
    }
    
    if (location && (::strlen(location) > 0))
    {
		len = ::strlen(finalLocation);
		
		if ((len > 0) && (finalLocation[len-1] != '/'))
		{
			finalLocation[len++] = '/';
			finalLocation[len] = '\0';
		}

		::strcat(finalLocation, location);
    }

    len = ::strlen(finalLocation);
    if ((len > 0) && (finalLocation[len-1] != '/'))
    {
		finalLocation[len++] = '/';
		finalLocation[len] = '\0';
    }
	
    return finalLocation;
}  

/*
 * Does this store need to do the full write_uncommitted/commit protocol
 */

Boolean ObjectStore::fullCommitNeeded () const
{
    return TRUE;
}

Boolean ObjectStore::isType ( const Uid& u, const TypeName tn, StateStatus st )
{
    return (((st & currentState(u,tn)) == st) ? TRUE : FALSE);
}

void ObjectStore::addToList ( SetUpInfo *creator )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void ObjectStore::addToList ( " << (void *)creator << " )" << endl;
    debug_stream.unlock();
    
#endif

    if (creator)
    {
		if (!headOfList)
			headOfList = creator;
		else
		{
			Boolean found = FALSE;
			SetUpInfo *marker = headOfList, *trail = 0;
			
			while ((!found) && (marker))
			{
#ifdef DEBUG
				debug_stream.lock();
				
				debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
				debug_stream << "ObjectStore::addToList - Comparing " << creator->_tn << " with " << marker->_tn << endl;
				debug_stream.unlock();
				
#endif
				/* neither tn should be 0 -	if they are they are ignored */
				
				if ((creator->_tn == 0) || (marker->_tn == 0) ||
					(::strcmp(creator->_tn, marker->_tn) != 0))
				{
					trail = marker;
					marker = marker->_next;
				}
				else
					found = TRUE;
			}
			
			if (!found)
				trail->_next = creator;
		}
    }
}


void ObjectStore::printList (ostream& toUse)
{
    SetUpInfo *marker = headOfList;

    toUse << "ObjectStore list contains:" << endl;
    
    while (marker)
    {
		toUse << marker->_tn << endl;
		marker = marker->_next;
    }

    toUse << "End of list." << endl;
}

ObjectStore* ObjectStore::create ( const TypeName name,
									const char* locationOfStore )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore* ObjectStore::create ( " << name << " )" << endl;
    debug_stream.unlock();
    
#endif

    if (headOfList)
    {    
		SetUpInfo* marker = headOfList;
    
		while (marker)
		{
			if (marker->_tn && ::strcmp(marker->_tn, name) == 0)
				return ::create(*marker, locationOfStore);
			else
				marker = marker->_next;
		}
    }

    error_stream << WARNING << "Cannot find the following type of object store: " << name
		 << "\n#include RemoteOSTypes.h or LocalOSTypes.h" << endl;
    return 0;
}

Boolean ObjectStore::reference ( ObjectStore* toRef )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean ObjectStore::reference (ObjectStore* toRef)" << endl;
    debug_stream.unlock();
    
#endif

    if (toRef == 0)
    {
        error_stream << WARNING << "ObjectStore::reference - call on null object." << endl;

		return FALSE;
    }

    if (headOfList)
    {    
		SetUpInfo* marker = headOfList;
    
		while (marker)
		{
			if (marker->_tn && ::strcmp(marker->_tn, toRef->type()) == 0)
				return ::reference(*marker,toRef);
			else
			marker = marker->_next;
		}
    }

    error_stream << FATAL << "Cannot find the following type of object store: " << toRef->type()
		 << "\n#include RemoteOSTypes.h or LocalOSTypes.h" << endl;

    return FALSE;
}

void ObjectStore::destroy ( ObjectStore*& toDelete )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void ObjectStore::destroy ( " << ((toDelete) ? toDelete->type() : "NULL") << " )" << endl;
    debug_stream.unlock();
    
#endif

    if (toDelete == 0)
		return;
    
    if (headOfList)
    {
		SetUpInfo* marker = headOfList;

		while (marker)
		{
			if (marker->_tn && ::strcmp(marker->_tn, toDelete->type()) == 0)
			{
				::destroy(*marker, toDelete->getStoreName(), toDelete);
				toDelete = 0;
				return;
			}
			else
				marker = marker->_next;
		}
    }
}

/*
 * The protected constructor
 */

ObjectStore::ObjectStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PROTECTED;
    debug_stream << "ObjectStore::ObjectStore ()" << endl;
    debug_stream.unlock();
    
#endif
}

#ifdef NO_INLINES
#  define OBJSTORE_CC_
#  include "ObjectStore/ObjStore.n"
#  undef OBJSTORE_CC_
#endif
