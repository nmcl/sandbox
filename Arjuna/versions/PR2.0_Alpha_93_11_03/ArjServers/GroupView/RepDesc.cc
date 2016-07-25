/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc.cc,v
 */


#include "RepDesc_stub.h"


#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
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

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef REPLICADATA_H_
#  include "ReplicaData.h"
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif


const ReplicaDescriptor NIL_RD;

extern const char* templateName;
extern const char* wildCardName;
extern const char* globalWildCardName;


ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor& rd)
                                                                 : ReplicaData (),
								   next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor& rd)" << endl;
#endif

    *this = rd;
}

ReplicaDescriptor::ReplicaDescriptor ()
                                      : ReplicaData (),
					next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor ()\n" << flush;
#endif
}

ReplicaDescriptor::~ReplicaDescriptor ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::~ReplicaDescriptor ()\n" << flush;
#endif

    if (next)
	delete next;
}

long ReplicaDescriptor::getNumber () const
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

ReplicaDescriptor* ReplicaDescriptor::getOwnTemplate (const Uid& u, Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaDescriptor* ReplicaDescriptor::getOwnTemplate ( " << u << " ) const" << endl;
#endif

    const ReplicaDescriptor *indx = this;
    ReplicaDescriptor *globalTP = 0, *marker = (ReplicaDescriptor*) this;
    Boolean found = FALSE, traversedReplica = FALSE;

    if (fromCache)
    {
	if (isGlobalWildCard())
	    globalTP = (ReplicaDescriptor*) this;
    }
    else
    {
	if ((isGlobalWildCard()) || (isWildCard()))
	    globalTP = (ReplicaDescriptor*) this;
    }
    
    /*
     * Allow multiple templates within composite group, so need to get template which applies
     * to this replica (group). If none found and global wild card template is present then
     * use that, otherwise fail.
     */

    while ((indx) && (!found))
    {
	if (indx->getGroupUid() == u)
	    found = TRUE;
	else
	{
	    if (isTemplate())
	    {
		// remember global wild card template.

		if (traversedReplica)
		{
		    marker = (ReplicaDescriptor*) indx;
		    traversedReplica = FALSE;

		    /*
		     * Object name will either be TemplateObject or WildCardObject
		     * If a globalWildCard template has been previously obtained then
		     * use this for group view. If "normal" wild card then go back to database
		     * in case replica group is mentioned there.
		     */

		    if (fromCache)
		    {
			if (isGlobalWildCard())
			    globalTP = marker;
		    }
		    else
		    {
			if ((isWildCard()) || (isGlobalWildCard()))
			    globalTP = marker;
		    }
		}
	    }
	    else
	    {
		if (!traversedReplica)
		    traversedReplica = TRUE;
	    }
	}
	indx = indx->next;
    }
    
    if (!found)
    {
	if (globalTP == 0)
	    return 0;
	else
	    marker = globalTP;
    }

    if (marker == 0)
	marker = (ReplicaDescriptor*) this;

    return marker;
}

ReplicaDescriptor* ReplicaDescriptor::getTemplate (const Uid& u, Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaDescriptor* ReplicaDescriptor::getTemplate ( " << u << " ) const" << endl;
#endif

    const ReplicaDescriptor *indx;
    ReplicaDescriptor *tmplate = 0, *marker = 0;

    if (!isTemplate())
	return 0;
    
    indx = getOwnTemplate(u, fromCache);

    while (indx)
    {
	if (indx->getActive())
	{
	    marker = new ReplicaDescriptor;
	    marker->next = tmplate;
	    marker->setHost((char*) indx->getHost());
	    tmplate = marker;
	    cout << "creating indx" << endl;
	    
	}
	    
	indx = indx->next;
    }

    return tmplate;
}

void ReplicaDescriptor::makeViewFromTemplate (const Uid& u, ReplicaDescriptor*& objects, Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void ReplicaDescriptor::makeViewFromTemplate ( " << u << " ) const" << endl;
#endif

    ReplicaDescriptor *marker = 0, *ptr = 0, *trail = 0, *indx = getTemplate(u, fromCache);
    
    if (indx == 0)
	return;
    else
	trail = indx;

    getReplicaView(u, objects);

    if (objects == 0)
	objects = new ReplicaDescriptor;

    ptr = objects;
    
    while (indx)
    {
	if (indx->getActive() == TRUE)
	{
	    Boolean hasUsed = FALSE;
		
	    while ((ptr) && (!hasUsed))
	    {
		// this is a template replica or a wildcard

		if ((::strcmp(ptr->getHost(), nullRDName) == 0))
		{
		    ptr->setHost((char*) indx->getHost());
		    if (ptr->getUid() == NIL_UID)
			ptr->setUid(u);
		    ptr->setGroupUid(u);
		    hasUsed = TRUE;
		}
		else
		{
		    if (::strcmp(ptr->getHost(), indx->getHost()) == 0)
			hasUsed = TRUE;
		    else
		    {
			marker = ptr;
			ptr = ptr->next;
		    }
		}
	    }
	    
	    if (!hasUsed)
	    {
		marker->next = new ReplicaDescriptor;
		ptr = marker->next;
		ptr->setHost((char*) indx->getHost());
		if (ptr->getUid() == NIL_UID)
		    ptr->setUid(u);
		ptr->setGroupUid(u);
	    }

	    marker = ptr;
	    ptr = ptr->next;
	}

	indx = indx->next;
    }

    delete trail;
}

void ReplicaDescriptor::testAndSetObjectNames (const TypeName tn)
{
    ReplicaDescriptor* tmpRD = this;
    
    while (tmpRD)
    {
	if (tmpRD->isNullType())
	    tmpRD->setObjectName(tn);
	tmpRD = tmpRD->next;
    }
}

void ReplicaDescriptor::getReplicaView (const Uid& u, ReplicaDescriptor*& objects) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "void ReplicaDescriptor::getReplicaView ( " << u << " ) const" << endl;
#endif

    ReplicaDescriptor* ptr = objects;

    /*
     * guarantee that as soon as first non-member is encountered
     * no more members will ever be encountered.
     */

    if ((u == getGroupUid()) && (!isTemplate()))
    {
        if (objects == 0)
	{
	    objects = new ReplicaDescriptor;
	    ptr = objects;
	}
	else
	{
	    objects->next = new ReplicaDescriptor;
	    ptr = objects->next;
	}

	ptr->setHost((char*) getHost());
	ptr->setUid(getUid());
	ptr->setGroupUid(getGroupUid());
	ptr->setObjectName((char*) getObjectName());
	ptr->setActive(getActive());
    }
    else
    {
        if (objects != 0)
	    return;
    }

    if (next != 0)
    {
        if (ptr != 0)
	    next->getReplicaView(u, ptr);
	else
	    next->getReplicaView(u, objects);
    }
}

void ReplicaDescriptor::getGroupList (const Uid& u, ReplicaDescriptor*& objects, Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* ReplicaDescriptor::getGroupList ( " << u << " ) const" << endl;
#endif

    /*
     * If this is a template object then construct real groupview here - saves time later.
     * Guarantee that template object appears at head of list, but need to check for
     * multiple template objects within composite list.
     */

    if (objects)
    {
	delete objects;
	objects = 0;
    }

    makeViewFromTemplate(u, objects, fromCache);

    // no relevant template found.

    if (objects == 0)
	getReplicaView(u, objects);
}

void ReplicaDescriptor::getCachedGroupList (const Uid& u, ReplicaDescriptor*& objects) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::getCachedGroupList (const Uid& u, ReplicaDescriptor*& objects) const" << endl;
#endif

    getGroupList(u, objects, TRUE);
}

Boolean ReplicaDescriptor::pack (Buffer& packInto, int sizeL) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaDescriptor::pack (Buffer& packInto) const" << endl;
#endif

    Boolean result = TRUE;

    if (sizeL == -1)
        result = packInto.pack(getNumber());
    result = result && ReplicaData::pack(packInto);
    if ((next) && (result))
        result = next->pack(packInto, 0);

    return result;
}

Boolean ReplicaDescriptor::unpack (Buffer& unpackFrom, int number)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaDescriptor::unpack (Buffer& unpackFrom)" << endl;
#endif

    Boolean result = TRUE;
    
    if (number == -1)
        result = unpackFrom.unpack(number);
    result = result && ReplicaData::unpack(unpackFrom);
    number--;

    if ((result) && (number > 0))
    {
        if (!next)
	    next = new ReplicaDescriptor;
	result = next->unpack(unpackFrom, number);
    }

    // This should never happen, but just in case ...
    if ((number == 0) && (next != 0))
    {
        ReplicaDescriptor *indx = next, *ptr = 0;
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

ostream& ReplicaDescriptor::print (ostream& strm) const
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


ReplicaDescriptor& ReplicaDescriptor::operator+= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator+=" << endl;
#endif

    ReplicaDescriptor* ptr = this;
    
    while (ptr->next)
	ptr = ptr->next;

    ptr->next = new ReplicaDescriptor;
    ptr = ptr->next;
    *ptr = gd;
    
    return *this;
}

ReplicaDescriptor& ReplicaDescriptor::operator+= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;   
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator+=" << endl;
#endif

    ReplicaDescriptor* ptr = this;

    if (&rd == this)
	return *this;
    
    while (ptr->next)
	ptr = ptr->next;
    
    ptr->next = new ReplicaDescriptor;
    ptr = ptr->next;
    *ptr = rd;
    
    return *this;
}

ReplicaDescriptor& ReplicaDescriptor::operator= (const GroupData& gd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator=" << endl;
#endif

    setHost((char*) gd.getHost());
    setObjectName((char*) gd.getObjectName());
    setUid(gd.getUid());
    setGroupUid(gd.getGroupUid());
    setActive(gd.getActive());

    if (gd.next)
    {
        if (next == 0)
	    next = new ReplicaDescriptor;
	*next = *gd.next;
    }

    return *this;
}

ReplicaDescriptor& ReplicaDescriptor::operator= (const ReplicaDescriptor& rd)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor& ReplicaDescriptor::operator=" << endl;
#endif

    if (&rd == this)
	return *this;

    setHost((char*) rd.getHost());
    setObjectName((char*) rd.getObjectName());
    setUid(rd.getUid());
    setGroupUid(rd.getGroupUid());
    setActive(rd.getActive());
    
    if (rd.next)
    {
	if (!next)
	    next = new ReplicaDescriptor;
	
	*next = *rd.next;
    }

    return *this;
}

Boolean ReplicaDescriptor::operator== (const ReplicaDescriptor& rd)
{
    Boolean result = FALSE;

    if (((next) && (rd.next)) || ((next == 0) && (rd.next == 0)))
    {
	if ((getUid() == rd.getUid()) && (getActive() == rd.getActive())
	    && (getGroupUid() == rd.getGroupUid()))
	{
	    if ((::strcmp(getHost(), rd.getHost()) == 0) && (::strcmp(getObjectName(), rd.getObjectName()) == 0))
	    {
	        if (next)
		  result = (Boolean) (*next == *rd.next);
		else
		  result = TRUE;
	    }
	}
    }
    
    return result;
}

Boolean ReplicaDescriptor::operator!= (const ReplicaDescriptor& rd)
{
    return ((*this == rd) ? FALSE : TRUE);
}

ostream& operator<< (ostream& strm, const ReplicaDescriptor& rd)
{
    return rd.print(strm);
}

void ReplicaDescriptor::unmarshall (RpcBuffer& rpcbuff)
{
    (void) unpack(rpcbuff);
}

void ReplicaDescriptor::marshall (RpcBuffer& rpcbuff) const
{
    (void) pack(rpcbuff);
}


RpcBuffer& operator<< (RpcBuffer& rpcbuff, ReplicaDescriptor* topack)
{
    const ReplicaDescriptor* temp = topack;
    return rpcbuff << temp;
}

RpcBuffer& operator<< (RpcBuffer& rpcbuff, ReplicaDescriptor& topack)
{
    ReplicaDescriptor* temp = &topack;
    return rpcbuff << temp;
}
