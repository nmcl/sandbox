/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaData.cc,v
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
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
#  include "TemplateObject.h"
#endif

#include "RepDesc_stub.h"

#ifndef REPLICADATA_H_
#  include "ReplicaData.h"
#endif


const char* nullRDName = "dummyHostName";
const char* nullObjectName = "server";


ReplicaData::ReplicaData ()
                          : destn_host(0),
			    objectName(0),
			    u(0),
			    groupUid(0),
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
    
    if (u)
	delete u;
    if (groupUid)
        delete groupUid;
    if (destn_host)
#ifndef __GNUG__
	::delete [] destn_host;
#else
        ::delete destn_host;
#endif
    if (objectName)
#ifndef __GNUG__
	::delete [] objectName;
#else
        ::delete objectName;
#endif
}

void ReplicaData::setHost (char* Name)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setHost ( " << Name << " )" << endl;
#endif

    if (Name)
    {
	int len = ::strlen(Name);
	
	if (destn_host)
#ifndef __GNUG__
	    ::delete [] destn_host;
#else
	    ::delete destn_host;
#endif
	destn_host = ::new char[len +1];
	::memset(destn_host, '\0', len+1);
	::memcpy(destn_host, Name, len);
    }
}

void ReplicaData::setObjectName (char* objName)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setObjectName ( " << objName << " )" << endl;
#endif

    if (objName)
    {
	int len = ::strlen(objName);
	
	if (objName)
#ifndef __GNUG__
	    ::delete [] objectName;
#else
	    ::delete objectName;
#endif
	objectName = ::new char[len+1];
	::memset(objectName, '\0', len+1);
	::memcpy(objectName, objName, len);
    }
}

void ReplicaData::setUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setUid ( " << u_id << " )" << endl;
#endif

    if (u)
	delete u;
    u = new Uid(u_id);
}

void ReplicaData::setGroupUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaData::setGroupUid ( " << u_id << " )" << endl;
#endif

    if (groupUid)
	delete groupUid;
    groupUid = new Uid(u_id);
}

Boolean ReplicaData::pack (Buffer& packInto) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::pack (Buffer& packInto) const" << endl;
#endif

    Boolean result = TRUE;

    result = result && packInto.pack(destn_host) && packInto.pack(objectName) && packInto.pack(active);
    result = result && u->pack(packInto) && groupUid->pack(packInto);
    return result;
}

Boolean ReplicaData::unpack (Buffer& unpackFrom)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::unpack (Buffer& unpackFrom)" << endl;
#endif

    Boolean result = TRUE;

    if (destn_host)
    {
#ifndef __GNUG__
        ::delete [] destn_host;
#else
        ::delete destn_host;
#endif
        destn_host = 0;
    }

    if (objectName)
    {
#ifndef __GNUG__
	::delete [] objectName;
#else
	::delete objectName;
#endif
	objectName = 0;
    }
    
    result = result && unpackFrom.unpack(destn_host) && unpackFrom.unpack(objectName) && unpackFrom.unpack(active);
    result = result && u->unpack(unpackFrom) && groupUid->unpack(unpackFrom);
    return result;
}

void ReplicaData::Initialize ()
{
    setHost((char*) nullRDName);
    setObjectName((char*) nullObjectName);
    setUid(NIL_UID);
    setGroupUid(NIL_UID);
    setActive(TRUE);
}

Boolean ReplicaData::isTemplate () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaData::isTemplate () const" << endl;
#endif

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
	strm << "Group : " << *groupUid << "\n";
	strm << "Uid : " << *u << "\n";
	strm << "destn_host : " << destn_host << "\n";
	strm << "Object type : " << objectName << "\n";
    
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
#  include <RPC/ArjServers/ReplicaData.n>
#  undef REPLICADATA_CC_
#endif
