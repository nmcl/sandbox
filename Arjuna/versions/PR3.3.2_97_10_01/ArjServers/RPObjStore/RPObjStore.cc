/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RPObjStore.cc,v 1.12 1995/09/05 14:11:23 nmcl Exp $
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define RPOBJSTORE_SERVER

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef CTYPE_H_
#  include <System/ctype.h>
#endif

#ifndef DIRENT_H_
#  include <System/dirent.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#include "RPObjStore_stub.h"



StoreManagerList* RPObjStore::headOfList = (StoreManagerList*) 0;


StoreManagerList::StoreManagerList (ObjectStore* toAdd)
				   : ptr(toAdd),
				     next(0)
{
}

StoreManagerList::~StoreManagerList ()
{
    if (ptr)
	ObjectStore::destroy(ptr);
    if (next)
	delete next;
}


RPObjStore::RPObjStore (int& res)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStore::RPObjStore ()" << endl;
#endif

    headOfList = (StoreManagerList*) 0;
    currentStore = (ObjectStore*) 0;
    buff = (ObjectState*) 0;
    res = 0;
}

RPObjStore::~RPObjStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStore::~RPObjStore ()" << endl;
#endif

    if (headOfList)
    {
	delete headOfList;
	headOfList = (StoreManagerList*) 0;
    }
}

/*
 * Check to see if an object store with this root exists. If it does
 * return a pointer to it for use, if not, add it to the list and
 * then return it.
 */

ObjectStore* RPObjStore::addToList (const char* storeRoot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "ObjectStore* RPObjStore::addToList ( " << storeRoot << " )" << endl;
#endif

    if (storeRoot == (char*) 0)
	storeRoot = "";
    
    if (headOfList == (StoreManagerList*) 0)
    {
	ObjectStore* toAdd = ObjectStore::create(DEFAULT_OBJECTSTORE_Q, storeRoot);
	headOfList = new StoreManagerList(toAdd);

	return headOfList->ptr;
    }
    else
    {
	StoreManagerList* trail = headOfList;

	while (trail)
	{
	    if (::strcmp(trail->ptr->getStoreName(), storeRoot) == 0)
		return trail->ptr;

	    trail = trail->next;
	}
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RPObjStore::addToList - adding new object store < " << DEFAULT_OBJECTSTORE_Q << ", " << storeRoot << " >" << endl;
#endif
    
    // must be new object store.

    ObjectStore* toAdd = ObjectStore::create(DEFAULT_OBJECTSTORE_Q, storeRoot);
    StoreManagerList* tmp = new StoreManagerList(toAdd);
    tmp->next = headOfList;
    headOfList = tmp;

    return headOfList->ptr;
}

/*
 * Public non-virtual functions
 */

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

Boolean RPObjStore::allObjUids (const TypeName tName, ObjectState& state, int st,
				const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStore::allObjUids (" << tName << ")\n" << flush;
#endif

    Boolean outcome = FALSE;

    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	outcome = currentStore->allObjUids(tName, state, (ObjectStore::StateStatus) st);
    
    return outcome;
}

int RPObjStore::currentState (const Uid& u, const TypeName tn, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "int RPObjStore::currentState ( " << u << ", " << tn << " )" << endl;
#endif

    int result = (int) ObjectStore::OS_UNKNOWN;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = (int) currentStore->currentState(u, tn);

    return result;
}
    
Boolean RPObjStore::commit_state (const Uid& objUid, const TypeName tName, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStore::commit_state (" << objUid << ", " << tName << ")\n" << flush;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);
    
    if (currentStore)
	result = currentStore->commit_state(objUid, tName);
    
    return result;
}

Boolean RPObjStore::hide_state (const Uid& u, const TypeName tn, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::hide_state ( " << u << ", " << tn << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->hide_state(u, tn);

    return result;
}

Boolean RPObjStore::reveal_state (const Uid& u, const TypeName tn, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::reveal_state ( " << u << ", " << tn << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->reveal_state(u, tn);

    return result;
}

ObjectState* RPObjStore::read_committed (const Uid& objUid, const TypeName tName, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* RPObjStore::read_committed ( " << objUid << ", " << tName << " )" << endl;
#endif

    buff = (ObjectState*) 0;
    currentStore = addToList(root);

    res = 0;
    if (currentStore)
	buff = currentStore->read_committed(objUid, tName);

    return buff;
}

ObjectState* RPObjStore::read_uncommitted (const Uid& objUid, const TypeName tName, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* RPObjStore::read_uncommitted ( " << objUid << ", " << tName << " )" << endl;
#endif

    buff = (ObjectState*) 0;
    currentStore = addToList(root);

    res = 0;
    if (currentStore)
	buff = currentStore->read_uncommitted(objUid, tName);

    return buff;
}

Boolean RPObjStore::remove_committed (const Uid& u, const TypeName name, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::remove_committed (" << u << ", " << name << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->remove_committed(u, name);

    return result;
}

Boolean RPObjStore::remove_uncommitted (const Uid& u, const TypeName name, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::remove_uncommitted ( " << u << ", " << name << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->remove_uncommitted(u, name);

    return result;
}

Boolean RPObjStore::write_committed (const Uid& objUid, const TypeName tName,
				     const ObjectState& state, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::write_committed ( " << objUid << ", " << tName << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->write_committed(objUid, tName, state);

    return result;
}

Boolean RPObjStore::write_uncommitted (const Uid& objUid, const TypeName tName,
					const ObjectState& state, const char* root, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean RPObjStore::write_uncommitted ( " << objUid << ", " << tName << " )" << endl;
#endif

    Boolean result = FALSE;
    
    res = 0;
    currentStore = addToList(root);

    if (currentStore)
	result = currentStore->write_uncommitted(objUid, tName, state);

    return result;
}
