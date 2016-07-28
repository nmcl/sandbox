/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaData.cc,v 1.15 1995/02/10 11:40:43 ngdp Exp $
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef TEMPLATEOBJECT_H_
#  include <ArjServers/TemplateObject.h>
#endif

#include "RepDesc_stub.h"

#ifndef REPLICADATA_STUB_H_
#  include "ReplicaData.h"
#endif


const char* nullRDName = "dummyHostName";
const char* nullObjectName = "server";


ReplicaData::ReplicaData ()
                          : destn_host(0),
			    objectName(0),
			    rootOfStore(0),
			    _uid(0),
			    _groupUid(0),
			    active(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaData::ReplicaData ()" << endl;
#endif

    Initialize();
}
   
ReplicaData::~ReplicaData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaData::~ReplicaData ()" << endl;
#endif
    
    if (_uid)
	delete _uid;
    if (_groupUid)
        delete _groupUid;
    if (destn_host)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] destn_host;
#else
        ::delete destn_host;
#endif
    if (objectName)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objectName;
#else
        ::delete objectName;
#endif
    if (rootOfStore)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] rootOfStore;
#else
        ::delete rootOfStore;
#endif    
}

void ReplicaData::setHost (const char* Name)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setHost ( " << Name << " )" << endl;
#endif

    if (Name)
    {
	int len = ::strlen(Name);
	
	if (destn_host)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] destn_host;
#else
	    ::delete destn_host;
#endif
	destn_host = ::new char[len +1];
	::memset(destn_host, '\0', len+1);
	::memcpy(destn_host, Name, len);
    }
}

void ReplicaData::setObjectName (const char* objName)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setObjectName ( " << objName << " )" << endl;
#endif

    if (objectName)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] objectName;
#else
        ::delete objectName;
#endif

    if (objName)
    {
	int len = ::strlen(objName);
	
	objectName = ::new char[len+1];
	::memset(objectName, '\0', len+1);
	::memcpy(objectName, objName, len);
    }
    else
	objectName = (char*) 0;
}

void ReplicaData::setStoreRoot (const char* root)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setStoreRoot ( " << root << " )" << endl;
#endif

    if (rootOfStore)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] rootOfStore;
#else
        ::delete rootOfStore;
#endif

    if (root == (char*) 0)
	root = "";
    
    int len = ::strlen(root);
	
    rootOfStore = ::new char[len+1];
    ::strcpy(rootOfStore, root);
}
	
void ReplicaData::setUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setUid ( " << u_id << " )" << endl;
#endif

    if (_uid)
	delete _uid;
    _uid = new Uid(u_id);
}

void ReplicaData::setGroupUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setGroupUid ( " << u_id << " )" << endl;
#endif

    if (_groupUid)
	delete _groupUid;
    _groupUid = new Uid(u_id);
}

Boolean ReplicaData::pack (Buffer& packInto, int) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::pack (Buffer& packInto) const" << endl;
#endif

    Boolean result = TRUE;

    result = result && packInto.packMappedString(destn_host) &&
	packInto.packMappedString(objectName);

    if (rootOfStore)
	result = result && packInto.pack(1) && packInto.packMappedString(rootOfStore);
    else
	result = result && packInto.pack(0);
    
    result = result && packInto.pack(active) && _uid->pack(packInto) && _groupUid->pack(packInto);
    
    return result;
}

Boolean ReplicaData::unpack (Buffer& unpackFrom, int)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::unpack (Buffer& unpackFrom)" << endl;
#endif

    Boolean result = TRUE;
    int packPtr = -1;

    if (destn_host)
    {
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] destn_host;
#else
        ::delete destn_host;
#endif
        destn_host = (char*) 0;
    }

    if (objectName)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objectName;
#else
	::delete objectName;
#endif
	objectName = (char*) 0;
    }

    if (rootOfStore)
    {
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] rootOfStore;
#else
        ::delete rootOfStore;
#endif
	rootOfStore = (char*) 0;
    }
    
    result = result && unpackFrom.unpack(destn_host) && unpackFrom.unpack(objectName);
    result = result && unpackFrom.unpack(packPtr);

    if (packPtr == 1)
	result = result && unpackFrom.unpack(rootOfStore);
    
    result = result && unpackFrom.unpack(active) && _uid->unpack(unpackFrom) && _groupUid->unpack(unpackFrom);

    return result;
}

void ReplicaData::Initialize ()
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void ReplicaData::Initialize ()" << endl;
#endif

    setHost(nullRDName);
    setObjectName(nullObjectName);
    setStoreRoot("");
    setUid(NIL_UID);
    setGroupUid(NIL_UID);
    setActive(TRUE);
}

Boolean ReplicaData::isTemplate () const
{
    if ((::strcmp(objectName, templateName) == 0) ||
	(::strcmp(objectName, wildCardName) == 0) ||
	(::strcmp(objectName, globalWildCardName) == 0))
	return TRUE;
    else
	return FALSE;
}

Boolean ReplicaData::isWildCard () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::isWildCard () const" << endl;
#endif

    return (Boolean) (::strcmp(objectName, wildCardName) == 0);
}

Boolean ReplicaData::isGlobalWildCard () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::isGlobalWildCard () const" << endl;
#endif

    return (Boolean) (::strcmp(objectName, globalWildCardName) == 0);
}

Boolean ReplicaData::isNullHost () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::isNullHost () const" << endl;
#endif

    return (Boolean) (::strcmp(destn_host, nullRDName) == 0);
}

Boolean ReplicaData::isNullType () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::isNullType () const" << endl;
#endif

    return (Boolean) (::strcmp(objectName, nullObjectName) == 0);
}

ostream& ReplicaData::print (ostream& strm) const
{
    if (isTemplate())
    {
	strm << "Template node : " << destn_host << "\n";
	if (::strcmp(objectName, wildCardName) == 0)
	    strm << "This is a wild card template\n";
	else
	    if (::strcmp(objectName, globalWildCardName) == 0)
		strm << "This is a global wild card template\n";
	if (active)
	    strm << "Template is active\n";
	else
	    strm << "Template is inactive\n";
    }
    else
    {
	strm << "Group : " << *_groupUid << "\n";
	strm << "Uid : " << *_uid << "\n";
	strm << "destn_host : ";
	if (::strcmp(destn_host, nullRDName) == 0)
	    strm << "undefined [template replica]";
	else
	    strm << destn_host;
	strm << "\nObject type : " << objectName;
	strm << "\nRoot of store : " << ((rootOfStore) ? rootOfStore : "[default]") << "\n" << endl;
    
	if (active)
	    strm << "Replica is active\n";
	else
	    strm << "Replica is inactive\n";
    }
	
    return strm;
}

// these do not do anything, as derived classes redefine

void ReplicaData::unmarshall (RpcBuffer&) {}

void ReplicaData::marshall (RpcBuffer&) const {}


#ifdef NO_INLINES
#  define REPLICADATA_CC_
#  include <ArjServers/ReplicaData.n>
#  undef REPLICADATA_CC_
#endif
