/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupData.cc
 */

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#include <Common/Error.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


#include "RepDesc_stub.h"

#include "GroupData.h"

const char* nullGDName = "dummyHostName";


GroupData::GroupData (char* Name)
                                : next(0),
				  u(0),
				  active(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupData::GroupData ( " << Name << " )" << endl;
#endif

    char* tempName;
    int length;
    
    if (Name)
	tempName = Name;
    else
	tempName = (char*) nullGDName;
    
    length = strlen(tempName);
    destn_host = new char[length +1];
    ::memset(destn_host, '\0', length +1);
    ::memcpy(destn_host, tempName, length);
}

GroupData::GroupData ()
                      : next(0),
			u(0),
			active(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupData::GroupData ()" << endl;
#endif
 
    int length = strlen(nullGDName);
    destn_host = new char[length +1];
    ::memset(destn_host, '\0', length +1);
    ::memcpy(destn_host, nullGDName, length);
}
   
GroupData::~GroupData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupData::~GroupData ()" << endl;
#endif
    
    if (u)
    {
	delete u;
	u = 0;
    }

    if (destn_host)
	delete destn_host;
}

void GroupData::SetHost (char* Name)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void GroupData::SetHost ( " << Name << " )" << endl;
#endif

    if (Name)
    {
	int len = strlen(Name);
	
	if (destn_host)
	    delete destn_host;
	
	destn_host = new char[len +1];
	::memset(destn_host, '\0', len+1);
	::memcpy(destn_host, Name, len);
    }
}

void GroupData::SetUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void GroupData::SetUid ( " << u_id << " )" << endl;
#endif

    if (u)
	delete u;
    
    u = new Uid(u_id);
}

void GroupData::SetActive (Boolean value)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void GroupData::SetActive ( " << value << " )" << endl;
#endif

    active = value;
}

char* GroupData::GetHost () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "char* GroupData::GetHost () const" << endl;
#endif

    return destn_host;
}

Uid* GroupData::GetUid () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Uid* GroupData::GetUid () const" << endl;
#endif

    return u;
}

Boolean GroupData::GetActive () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::GetActive () const" << endl;
#endif

    return active;
}

GroupData& GroupData::operator= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "GroupData& GroupData::operator=" << endl;
#endif

    if (destn_host)
    {
	delete destn_host;
	destn_host = 0;
    }

    if (rd.GetHost())
    {
	int len = strlen(rd.GetHost());
	
	destn_host = new char[len +1];
	::memset(destn_host, '\0', len +1);
	::memcpy(destn_host, rd.GetHost(), len);
    }
    
    if (u)
    {
	delete u;
	u = 0;
    }
    
    if (rd.GetUid())
	u = new Uid(*rd.GetUid());
    active = rd.GetActive();
    
    if (rd.next)
    {
	next = new GroupData(0); // blank - will be filled in by next line
	*next = *rd.next;
    }
    
    return *this;
}

Boolean GroupData::operator== (const ReplicaDescriptor& rd)
{
    if ((*rd.GetUid() == *u) && (strcmp(destn_host, rd.GetHost()) == 0))
	return TRUE;
    else
	return FALSE;
}

ostream& GroupData::print (ostream& strm) const
{
    strm << "destn_host : " << destn_host << endl;
    
    if (u)
	strm << "Uid : " << *u << endl;
    else
	strm << "Uid : NULL" << endl;
    
    if (active)
	strm << "Replica is active" << endl;
    else
	strm << "Replica is inactive" << endl;
    
    if (next)
	strm << next;
    else
	strm << "End of list" << endl;
    
    return strm;
}

ostream& operator<< (ostream& strm, const GroupData& gd)
{
    return gd.print(strm);
}
