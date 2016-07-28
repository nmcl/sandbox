/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RepDesc.cc,v 1.12 1995/10/16 11:21:32 ngdp Exp $
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

#ifndef REPLICADATA_STUB_H_
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


ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor* rd)
    : ReplicaData(),
      next(0),
      groupPtr(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor* rd)" << endl;
#endif

    if (rd)
    {
	setHost(rd->getHost());
	setObjectName(rd->getObjectName());
	setStoreRoot(rd->getStoreRoot());
	setUid(rd->getUid());
	setGroupUid(rd->getGroupUid());
	setActive(rd->getActive());
    }
}

ReplicaDescriptor::ReplicaDescriptor (const GroupData* gd)
    : ReplicaData(),
      next(0),
      groupPtr(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor (const GroupData* gd)" << endl;
#endif

    if (gd)
    {
	setHost(gd->getHost());
	setObjectName(gd->getObjectName());
	setStoreRoot(gd->getStoreRoot());
	setUid(gd->getUid());
	setGroupUid(gd->getGroupUid());
	setActive(gd->getActive());
    }
}

ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor& rd)
    : ReplicaData (),
      next(0),
      groupPtr(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor::ReplicaDescriptor (const ReplicaDescriptor& rd)" << endl;
#endif

    *this = rd;
}

ReplicaDescriptor::ReplicaDescriptor ()
                                      : ReplicaData (),
					next(0),
                                        groupPtr(0)
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

/*
 * We do not check for duplicates.
 */

ReplicaDescriptor* ReplicaDescriptor::addToList (const ReplicaDescriptor& toAdd)
{
    ReplicaDescriptor* ptr = this;
 
    if (this == &toAdd)  // essentially an = operator, so do usual check.
	return this;
   
    while (ptr->next)
	ptr = ptr->next;
    
    ptr->next = new ReplicaDescriptor;
    *ptr->next = toAdd;

    return ptr->next;
}

ReplicaDescriptor* ReplicaDescriptor::addToList (const GroupData& toAdd)
{
    ReplicaDescriptor* ptr = this;
    
    while (ptr->next)
	ptr = ptr->next;
    
    ptr->next = new ReplicaDescriptor;
    *ptr->next = toAdd;

    return ptr->next;
}

/*
 * This method returns the number of elements in the list. Since
 * it counts all elements, templates included, it can give a
 * false impression of the size of a group. As such, it
 * is recommended that this method only be used when group views
 * have been formed (i.e., call getGroupList and then use this
 * method on the group view returned).
 */

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

    const ReplicaDescriptor* indx     = this;
    ReplicaDescriptor*       globalTP = (ReplicaDescriptor*) 0;
    ReplicaDescriptor*       marker   = (ReplicaDescriptor*) this;
    Boolean                  found    = FALSE, traversedReplica = FALSE;

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
     * Allow multiple templates within composite group, so need to get template
     * which applies to this replica (group). If none found and global wild
     * card template is present then use that, otherwise fail.
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
		     * Object name will either be TemplateObject or
		     * WildCardObject. If a globalWildCard template has been
	    	     * previously obtained then use this for group view. If
	             * "normal" wild card then go back to database in case
	             * replica group is mentioned there.
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
	if (globalTP == (ReplicaDescriptor*) 0)
	    return (ReplicaDescriptor*) 0;
	else
	    marker = globalTP;
    }

    if (marker == (ReplicaDescriptor*) 0)
	marker = (ReplicaDescriptor*) this;

    return marker;
}

Boolean ReplicaDescriptor::isMemberOfList (const Uid& groupToCheck) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaDescriptor::isMemberOfList ( " << groupToCheck << " ) const" << endl;
#endif

    if (!isTemplate() && (getGroupUid() == groupToCheck))
	return TRUE;
    else
	return ((next == (ReplicaDescriptor*) 0) ? FALSE : next->isMemberOfList(groupToCheck));
}

Boolean ReplicaDescriptor::templateMember (const Uid& templateUid) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaDescriptor::templateMember ( " << templateUid << " ) const" << endl;
#endif
    
    if (isTemplate() && (getGroupUid() == templateUid))
	return TRUE;
    else
	return ((next == (ReplicaDescriptor*) 0) ? FALSE : next->templateMember(templateUid));
}

void ReplicaDescriptor::reset ()
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::reset ()" << endl;
#endif

    groupPtr = (ReplicaDescriptor*) 0;
}

void ReplicaDescriptor::getNextGroup (const ReplicaDescriptor*& toUse)
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::getNextGroup (const ReplicaDescriptor*& toUse)" << endl;
#endif

    Uid* groupUid = new Uid(groupPtr->getGroupUid());;
    Boolean found = FALSE;

    if (groupPtr == (ReplicaDescriptor*) 0)
        groupPtr = this;

    while ((groupPtr) && (groupPtr->isTemplate()))
        groupPtr = groupPtr->next;

    if (groupPtr == (ReplicaDescriptor*) 0)
        return;

    while ((!found) && (groupPtr))
    {
        if (groupPtr->getGroupUid() != *groupUid)
	{
	    delete groupUid;
	    groupUid = new Uid(groupPtr->getGroupUid());
	    found = TRUE;
	}
	else
	    groupPtr = groupPtr->next;
    }

    if (found)
        toUse = groupPtr;
}

/*
 * This will return the *first* template encountered.
 */

void ReplicaDescriptor::getTemplate (GroupData*& templateView) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::getTemplate (GroupData*& templateView) const" << endl;
#endif

    const ReplicaDescriptor* trail = this;
    GroupData* tmpGD = (GroupData*) 0;
    Boolean done = FALSE;

    while ((trail) && (!done))
    {
        if (trail->isTemplate())
	{
	    tmpGD = new GroupData;
	    *tmpGD = *trail;
	    tmpGD->next = templateView;
	    templateView = tmpGD;
	    trail = trail->next;
	}
	else
	    done = TRUE;
    }
}

ReplicaDescriptor* ReplicaDescriptor::getTemplate (const Uid& u, Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "ReplicaDescriptor* ReplicaDescriptor::getTemplate ( " << u << " ) const" << endl;
#endif

    const ReplicaDescriptor* indx = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* tmplate = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* marker = (ReplicaDescriptor*) 0;

    if (!isTemplate())
	return (ReplicaDescriptor*) 0;
    
    indx = getOwnTemplate(u, fromCache);

    while (indx)
    {
	if (indx->getActive() && indx->isTemplate())
	{
	    marker = new ReplicaDescriptor;
	    marker->next = tmplate;
	    marker->setHost(indx->getHost());
	    tmplate = marker;
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

    ReplicaDescriptor* marker = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* ptr    = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* trail  = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* indx   = getTemplate(u, fromCache);
    
    if (indx == (ReplicaDescriptor*) 0)
	return;
    else
	trail = indx;

    getReplicaView(u, objects);

    if (objects == (ReplicaDescriptor*) 0)
	objects = new ReplicaDescriptor;

    ptr = objects;

    /*
     * Template gives number of replicas. objects gives their possible names.
     * Therefore, make sure we have enough entries for this.
     */

    while (indx)
    {
	if (indx->getActive() == TRUE)
	{
	    Boolean hasUsed = FALSE;
		
	    while ((ptr) && (!hasUsed))
	    {
	        /*
		 * If the host field for this replica is set then it cannot
		 * be governed by the template. This may be because it is
		 * a previously excluded replica from the template.
		 */

		if (::strcmp(ptr->getHost(), indx->getHost()) == 0)
		    hasUsed = TRUE;
		else
		{
		    // Is this a template?

		    if ((::strcmp(ptr->getHost(), nullRDName) == 0))
		    {
			ptr->setHost(indx->getHost());

			// check Uid just in case it is NIL (should not be!)
			if (ptr->getUid() == NIL_UID)
			    ptr->setUid(u);
			ptr->setGroupUid(u);
			hasUsed = TRUE;
		    }
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
		ptr->setHost(indx->getHost());
		ptr->setUid(u);
		ptr->setGroupUid(u);
		hasUsed = TRUE;
	    }

	    marker = ptr;
	    ptr = ptr->next;
	}

	indx = indx->next;
    }

    /*
     * We have too many "names" for the number of replicas mentioned
     * by the template. Get rid of them.
     */

    if (ptr != (ReplicaDescriptor*) 0)
    {
	marker->next = (ReplicaDescriptor*) 0;
	delete ptr;
    }

    objects->testAndSetObjectNames((const TypeName) objects->getObjectName());
    delete trail;
}

void ReplicaDescriptor::setAllHostNames (const char* node)
{
    ReplicaDescriptor* tmpRD = this;
    
    while (tmpRD)
    {
	tmpRD->setHost(node);
	tmpRD = tmpRD->next;
    }
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
        if (objects == (ReplicaDescriptor*) 0)
	{
	    objects = new ReplicaDescriptor;
	    ptr = objects;
	}
	else
	{
	    objects->next = new ReplicaDescriptor;
	    ptr = objects->next;
	}

	ptr->setHost(getHost());
	ptr->setUid(getUid());
	ptr->setGroupUid(getGroupUid());
	ptr->setObjectName(getObjectName());
	ptr->setStoreRoot(getStoreRoot());
	ptr->setActive(getActive());
    }
    else
    {
        if (objects != (ReplicaDescriptor*) 0)
	    return;
    }

    if (next != (ReplicaDescriptor*) 0)
    {
        if (ptr != (ReplicaDescriptor*) 0)
	    next->getReplicaView(u, ptr);
	else
	    next->getReplicaView(u, objects);
    }
}

/*
 * This routine returns the entire exclude list from the group view obtained
 * from the db. Since this group may be a composite or template group then
 * the excluded replicas could belong to several different groups.
 */

void ReplicaDescriptor::getExcludedList (ReplicaDescriptor*& excludeList) const
{
    const ReplicaDescriptor* trail = this;
    
    if (excludeList)
    {
	delete excludeList;
	excludeList = (ReplicaDescriptor*) 0;
    }
    
    while (trail)
    {
	if (trail->getActive() == FALSE)
	{
	    ReplicaDescriptor* toAdd = new ReplicaDescriptor(trail);
	    toAdd->next = excludeList;
	    excludeList = toAdd;
	}
	
	trail = trail->next;
    }
}

/*
 * This routine returns the excluded replicas only for the group given.
 */

void ReplicaDescriptor::getExcludedList (const Uid& groupUid, ReplicaDescriptor*& excludeList) const
{
    const ReplicaDescriptor* trail = this;
    
    if (excludeList)
    {
	delete excludeList;
	excludeList = (ReplicaDescriptor*) 0;
    }
    
    while (trail)
    {
	if ((trail->getActive() == FALSE) && (trail->getGroupUid() == groupUid))
	{
	    ReplicaDescriptor* toAdd = new ReplicaDescriptor(trail);
	    toAdd->next = excludeList;
	    excludeList = toAdd;
	}
	
	trail = trail->next;
    }
}

/*
 * This routine returns the available list for the group given. We have to
 * use this because the "group view" obtained from the db could contain
 * more than one actual group view if the group is a composite object, or
 * it may contain a template if the group is governed by one. Therefore, this
 * routine hides these details and simply returns the relevant group view.
 */

void ReplicaDescriptor::getGroupList (const Uid& u,
				      ReplicaDescriptor*& objects,
				      Boolean fromCache) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* ReplicaDescriptor::getGroupList ( " << u << " ) const" << endl;
#endif

    /*
     * If this is a template object then construct real groupview here - saves
     * time later. Guarantee that template object appears at head of list, but
     * need to check for multiple template objects within composite list.
     */

    ReplicaDescriptor* t_rd   = (ReplicaDescriptor*) 0;
    ReplicaDescriptor* toKeep = (ReplicaDescriptor*) 0;

    if (objects)
    {
	delete objects;
	objects = (ReplicaDescriptor*) 0;
    }

    makeViewFromTemplate(u, t_rd, fromCache);

    // no relevant template found.

    if (t_rd == (ReplicaDescriptor*) 0)
	getReplicaView(u, t_rd);

    // now remove any excluded replicas

    while (t_rd)
    {
        if (t_rd->getActive())
	{
	    toKeep = t_rd;
	    t_rd = t_rd->next;
	    toKeep->next = objects;
	    objects = toKeep;
	}
	else
	{
	    toKeep = t_rd->next;
	    t_rd->next = (ReplicaDescriptor*) 0;
	    delete t_rd;
	    t_rd = toKeep;
	}
    }
}

/*
 * This routine returns all replicas (excluded and included) for the given
 * group.
 */

void ReplicaDescriptor::getAllMembers (const Uid& u, ReplicaDescriptor*& objects, Boolean keepMinimum) const
{
#ifdef DEBUG
    debug_stream << OPERATORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void ReplicaDescriptor::getAllMembers ( " << u << " ) conts" << endl;
#endif

    const ReplicaDescriptor* tmpRD        = this;
    Boolean                  endOfList    = FALSE;
    Boolean                  addedReplica = FALSE;

    if (objects)
    {
        delete objects;
	objects = (ReplicaDescriptor*) 0;
    }

    /*
     * We end the search as soon as we encounter the first non-group
     * member after encountering the first group member.
     */

    while ((tmpRD) && (!endOfList))
    {
        if (tmpRD->getGroupUid() == u)
	{
	    /*
	     * If we find a replica which has a Uid the same as the group Uid
	     * then this is probably from a template, and we need to know
	     * whether we should return this or not. The user could have
	     * deliberately inserted a replica with this property though, so
	     * we cannot delete all copies. However, to try to keep the group
	     * to a minimum, we only allow one such replica in the group - the
	     * rest can be generated when required.
	     */

	    Boolean doInsert = (Boolean) ((!keepMinimum) || (tmpRD->getGroupUid() != tmpRD->getUid()));
	    
	    if ((doInsert) || ((tmpRD->getGroupUid() == tmpRD->getUid()) && (!addedReplica)))
	    {
	        ReplicaDescriptor* toAdd = new ReplicaDescriptor(tmpRD);
		toAdd->next = objects;
		objects = toAdd;

		if (!addedReplica)
		    addedReplica = (Boolean) (tmpRD->getGroupUid() == tmpRD->getUid());
	    }
	}
	else
	    if (objects)
	        endOfList = TRUE;

	tmpRD = tmpRD->next;
    }
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
    
    if ((number == 0) && (next != (ReplicaDescriptor*) 0))
    {
        ReplicaDescriptor* indx = next;
	ReplicaDescriptor* ptr = (ReplicaDescriptor*) 0;
	while (indx)
	{
	    ptr = indx;
	    indx = indx->next;
	    delete ptr;
	}
	next = (ReplicaDescriptor*) 0;
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

/*
 * This routine simply adds elements to the end of the list. Since there
 * is no point in having a list which is composed of multiple copies of
 * itself we do not allow this.
 */

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

    setHost(gd.getHost());
    setObjectName(gd.getObjectName());
    setStoreRoot(gd.getStoreRoot());
    setUid(gd.getUid());
    setGroupUid(gd.getGroupUid());
    setActive(gd.getActive());

    if (gd.next)
    {
        if (next == (ReplicaDescriptor*) 0)
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

    setHost(rd.getHost());
    setObjectName(rd.getObjectName());
    setStoreRoot(rd.getStoreRoot());
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

    if (((next) && (rd.next)) || ((next == (ReplicaDescriptor*) 0) && (rd.next == (ReplicaDescriptor*) 0)))
    {
	if ((getUid() == rd.getUid()) && (getActive() == rd.getActive())
	    && (getGroupUid() == rd.getGroupUid()))
	{
	    if ((::strcmp(getHost(), rd.getHost()) == 0) &&
		(::strcmp(getObjectName(), rd.getObjectName()) == 0))
	    {
		if ((getStoreRoot() && rd.getStoreRoot()) || (!getStoreRoot() && !rd.getStoreRoot()))
		{
		    result = TRUE;
		    
		    if (getStoreRoot() && rd.getStoreRoot())
		    {
			if (::strcmp(getStoreRoot(), rd.getStoreRoot()) != 0)
			    result = FALSE;
		    }

		    if ((next) && (result))
			result = (Boolean) (*next == *rd.next);
		}
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
