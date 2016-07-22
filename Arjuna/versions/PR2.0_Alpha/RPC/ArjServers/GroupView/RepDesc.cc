/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc.cc
 */

#include "RepDesc_stub.h"

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


#ifndef GROUPDATA_H_
#include "GroupData.h"
#endif

const char* nullRDName = "dummyHostName";


ReplicaDescriptor::ReplicaDescriptor (char* Name)
                                                : next(0),
						  u(0),
						  active(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor ( " << Name << " )\n" << flush;
#endif

    char* tempName;
    int length;

    if (Name)
	tempName = Name;
    else
	tempName = (char*) nullRDName;
    
    length = strlen(tempName);
    destn_host = new char[length+1];
    ::memset(destn_host, '\0', length+1);
    ::memcpy(destn_host, tempName, length);
}

ReplicaDescriptor::ReplicaDescriptor ()
                                      : next(0),
					u(0),
					active(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor ()\n" << flush;
#endif

    int length = strlen(nullRDName);
    destn_host = new char[length+1];
    ::memset(destn_host, '\0', length+1);
    ::memcpy(destn_host, nullRDName, length);
}

ReplicaDescriptor::~ReplicaDescriptor ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::~ReplicaDescriptor ()\n" << flush;
#endif

    if (next)
    {
	delete next;
	next = 0;
    }

    if (u)
    {
	delete u;
	u = 0;
    }

    if (destn_host)
	delete destn_host;
}

void ReplicaDescriptor::SetHost (char* Name)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::SetHost ( " << Name << " )" << endl;
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

void ReplicaDescriptor::SetUid (const Uid& u_id)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::SetUid ( " << u << " )" << endl;
#endif

    if (u)
	delete u;
    
    u = new Uid(u_id);
}

void ReplicaDescriptor::SetActive (Boolean value)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::SetActive ( " << value << " )" << endl;
#endif

    active = value;
}

char* ReplicaDescriptor::GetHost () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "char* ReplicaDescriptor::GetHost () const" << endl;
#endif

    return destn_host;
}

Uid* ReplicaDescriptor::GetUid () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Uid* ReplicaDescriptor::GetUid () const" << endl;
#endif

    return u;
}

Boolean ReplicaDescriptor::GetActive () const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaDescriptor::GetActive () const" << endl;
#endif

    return active;
}

ReplicaDescriptor& ReplicaDescriptor::operator= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator= (const GroupData& gd)" << endl;
#endif

    if (destn_host)
    {
	delete destn_host;
	destn_host = 0;
    }
    
    if (gd.GetHost())
    {
	int len = strlen(gd.GetHost());
	
	destn_host = new char[len +1];
	::memset(destn_host, '\0', len +1);
	::memcpy(destn_host, gd.GetHost(), len);
    }
    
    if (u)
    {
	delete u;
	u = 0;
    }
    
    if (gd.GetUid())
	u = new Uid(*gd.GetUid());
    active = gd.GetActive();
    
    if (gd.next)
    {
	next = new ReplicaDescriptor(0);
	*next = *gd.next;
    }

    return *this;
}

ReplicaDescriptor& ReplicaDescriptor::operator= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator=" << endl;
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
	next = new ReplicaDescriptor(0);
	*next = *rd.next;
    }

    return *this;
}

long ReplicaDescriptor::GetNumber () const
{
    long number = 0;
    const ReplicaDescriptor* ptr = this;
    
    while (ptr)
    {
	number++;
	ptr = ptr->next;
    }
    
    return number;
}
    
Boolean ReplicaDescriptor::operator== (const ReplicaDescriptor& rd)
{
    Boolean result = FALSE;

    if ((next) && (rd.next))
    {
	if ((*u == *rd.GetUid()) && (active == rd.GetActive()))
	{
	    if (strcmp(destn_host, rd.GetHost()) == 0)
		result = (Boolean) (*next == *rd.next);
	}
    }
    
    return result;
}

ostream& ReplicaDescriptor::print (ostream& strm) const
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

ostream& operator<< (ostream& strm, const ReplicaDescriptor& rd)
{
    return rd.print(strm);
}
