/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupData.cc,v 1.12 1994/10/14 10:43:35 nmcl Exp $
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

#include "RepDesc_stub.h"

#ifndef REPLICADATA_STUB_H_
#  include "ReplicaData.h"
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

const GroupData NIL_GD;

GroupData::GroupData () 
                      : ReplicaData (),
			next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData::GroupData ()" << endl;
#endif
}

GroupData::GroupData (const GroupData& gd)
                                         : ReplicaData(),
					   next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData::GroupData (const GroupData& gd)" << endl;
#endif
    
    *this = gd;
}

GroupData::GroupData (const ReplicaDescriptor& rd)
                                                : ReplicaData(),
                                                  next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData::GroupData (const ReplicaDescriptor& rd)" << endl;
#endif

    setHost(rd.getHost());
    setObjectName(rd.getObjectName());
    setStoreRoot(rd.getStoreRoot());
    setUid(rd.getUid());
    setGroupUid(rd.getGroupUid());
    setActive(rd.getActive());
}

GroupData::~GroupData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData::~GroupData ()" << endl;
#endif
}

long GroupData::getNumber () const
{
    long number = 0;
    const GroupData* ptr = this;

    while (ptr)
    {
        number++;
	ptr = ptr->next;
    }

    return number;
}

Boolean GroupData::isMemberOfList (const Uid& u) const
{
    if (getGroupUid() == u)
       return TRUE;
    else
        return ((next == (GroupData*) 0) ? FALSE : next->isMemberOfList(u));
}

Boolean GroupData::pack (Buffer& packInto, int sizeL) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::pack (Buffer& packInto) const" << endl;
#endif

    Boolean result = TRUE;

    if (sizeL == -1)
        result = packInto.pack(getNumber());
    result = result && ReplicaData::pack(packInto);
    if ((next) && (result))
        result = next->pack(packInto, 0);

    return result;
}

Boolean GroupData::unpack (Buffer& unpackFrom, int number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::unpack (Buffer& unpackFrom)" << endl;
#endif

    Boolean result = TRUE;
    
    if (number == -1)
        result = unpackFrom.unpack(number);

    result = result && ReplicaData::unpack(unpackFrom);
    number--;

    if ((result) && (number > 0))
    {
        if (!next)
	    next = new GroupData;
	result = next->unpack(unpackFrom, number);
    }

    // This should never happen, but just in case ...
    if ((number == 0) && (next != 0))
    {
        GroupData *indx = next, *ptr = 0;
	while (indx)
	{
	    ptr = indx;
	    indx = indx->next;
	    delete ptr;
	}
	next = 0;
    }

    return result;
}

GroupData* GroupData::assembleView (const GroupData& mask, GroupData* list)
{
    GroupData* dummy = 0;

    // build up list and return head

    while (list)
    {
	GroupData* tmp = new GroupData;
	*tmp = mask;
	tmp->setObjectName(list->getObjectName());
	tmp->setStoreRoot(list->getStoreRoot());
	tmp->setUid(list->getUid());
	tmp->setGroupUid(list->getGroupUid());
	tmp->setActive(list->getActive());
	tmp->next = dummy;
	dummy = tmp;
	
	list = list->next;
    }
    
    return dummy;
}
	
ostream& GroupData::print (ostream& strm) const
{
    ReplicaData::print(strm);
    if (next)
    {
	strm << "****\n";
	strm << *next;
    }
    else
	strm << "****" << endl;
    
    return strm;
}

GroupData& GroupData::operator= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData& GroupData::operator=" << endl;
#endif

    setHost(rd.getHost());
    setObjectName(rd.getObjectName());
    setStoreRoot(rd.getStoreRoot());
    setUid(rd.getUid());
    setGroupUid(rd.getGroupUid());
    setActive(rd.getActive());
    return *this;
}

GroupData& GroupData::operator= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData& GroupData::operator=" << endl;
#endif

    if (&gd == this)
	return *this;

    setHost(gd.getHost());
    setObjectName(gd.getObjectName());
    setStoreRoot(gd.getStoreRoot());
    setUid(gd.getUid());
    setGroupUid(gd.getGroupUid());
    setActive(gd.getActive());
    return *this;
}

/*
 * This routine simply adds elements to the end of the list. Since there
 * is no point in having a list which is composed of multiple copies of
 * itself we do not allow this.
 */
    
GroupData& GroupData::operator+= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "GroupData& GroupData::operator+=" << endl;
#endif

    GroupData* tempPtr = this;
    const GroupData* indx = &gd;
    
    if (&gd == this)
        return *this;

    while (tempPtr->next)
	tempPtr = tempPtr->next;
    
    while (indx)
    {
	if (tempPtr->next == 0)
	    tempPtr->next = new GroupData;
	
	tempPtr = tempPtr->next;
	*tempPtr = *indx;
	indx = indx->next;
    }
    
    return *this;
}

/*
 * Neither of these routines compares the availability of the objects.
 */

Boolean GroupData::operator== (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::operator==" << endl;
#endif

    if ((gd.getUid() == getUid()) && (::strcmp(getHost(), gd.getHost()) == 0)
	&& (::strcmp(getObjectName(), gd.getObjectName()) == 0) && (gd.getGroupUid() == getGroupUid()))
	{
	    if ((getStoreRoot() && gd.getStoreRoot()) || (!getStoreRoot() && !gd.getStoreRoot()))
	    {
		if (getStoreRoot() && gd.getStoreRoot())
		{
		    if (::strcmp(getStoreRoot(), gd.getStoreRoot()) == 0)
			return TRUE;
		    else
			return FALSE;
		}
		else
		    return TRUE;
	    }
	}

    return FALSE;
}
    
Boolean GroupData::operator== (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::operator==" << endl;
#endif

    if ((rd.getUid() == getUid()) && (::strcmp(getHost(), rd.getHost()) == 0)
	&& (::strcmp(getObjectName(), rd.getObjectName()) == 0) && (rd.getGroupUid() == getGroupUid()))
	{
	    if ((getStoreRoot() && rd.getStoreRoot()) || (!getStoreRoot() && !rd.getStoreRoot()))
	    {
		if (getStoreRoot() && rd.getStoreRoot())
		{
		    if (::strcmp(getStoreRoot(), rd.getStoreRoot()) == 0)
			return TRUE;
		    else
			return FALSE;
		}
		else
		    return TRUE;
	    }
	}

    return FALSE;
}

Boolean GroupData::operator!= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::operator!=" << endl;
#endif

    return ((*this == rd) ? FALSE : TRUE);
}

Boolean GroupData::operator!= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean GroupData::operator!=" << endl;
#endif

    return ((*this == gd) ? FALSE : TRUE);
}

ostream& operator<< (ostream& strm, const GroupData& gd)
{
    return gd.print(strm);
}

/*
 * Static member to delete entire list of elements.
 */

void GroupData::deleteAll (GroupData*& toDelete)
{
    GroupData* tmpGD = toDelete;
    
    while (toDelete)
    {
	toDelete = tmpGD->next;
	delete tmpGD;
	tmpGD = toDelete;
    }
    
    toDelete = (GroupData*) 0;
}

