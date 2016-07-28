/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TemplateObject.cc,v 1.6 1994/07/20 13:32:32 nmcl Exp $
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef TEMPLATEOBJECT_H_
#  include <ArjServers/TemplateObject.h>
#endif

#ifndef REPDESC_H_
#  include "RepDesc.h"
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


/*
 * The various template which exist are as follows:
 * TemplateObject : this is a "normal" template for a group, which indicates
 *                  where the replicas are to be located. It matches only those
 *                  groups explicitly mentioned at the database.
 * WildCard       : this template type matches any replica group which does not
 *                  have an explicit mention elsewhere at the database.
 * GlobalWildCard : as with a WildCard template this type matches any replica
 *                  group. The difference comes with the caching policy used.
 *                  Once a GlobalWildCard template is cached it will be used
 *                  in preference to going to the database for any subsequent
 *                  group whose replica information has not already been
 *                  cached. This template type should be used with great care.
 *
 * Note: because it is possible to have multiple object stores in the system
 * and the relative roots are necessary for each replica to find its state,
 * this additionla information could be stored as part of the template. However,
 * this would then force all members of the template group to use the same
 * (relative) object store location. Therefore, currently this information must
 * still be specified on a per member basis.
 */

const char* templateName = "TemplateObject";
const char* wildCardName = "WildCardTemplate";
const char* globalWildCardName = "GlobalWildCardTemplate";


TemplateObject::TemplateObject ()
                                : tmplate(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "TemplateObject::TemplateObject ()" << endl;
#endif
}

TemplateObject::~TemplateObject ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "TemplateObject::~TemplateObject ()" << endl;
#endif

    GroupData::deleteAll(tmplate);
}

ReplicaDescriptor* TemplateObject::getTemplate () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* TemplateObject::getTemplate () const" << endl;
#endif

    ReplicaDescriptor* x = new ReplicaDescriptor;
    *x = *tmplate;
    return x;
}

long TemplateObject::sizeofTemplate () const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "long TemplateObject::sizeofTemplate () const" << endl;
#endif

    return ((tmplate == (GroupData*) 0) ? 0 : tmplate->getNumber());
}
    
void TemplateObject::addTemplate (const char* hostname)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void TemplateObject::addTemplate ( " << hostname << " )" << endl;
#endif

    GroupData* ptr = new GroupData;

    ptr->next = tmplate;
    tmplate = ptr;

    tmplate->setHost(hostname);
}

void TemplateObject::deleteTemplate (const char* hostname)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void TemplateObject::deleteTemplate ( " << hostname << " )" << endl;
#endif

    GroupData* ptr   = tmplate;
    GroupData* indx  = (GroupData*) 0;
    Boolean    found = FALSE;

    if (ptr == (GroupData*) 0)
        return;

    while ((ptr) && (!found))
    {
        if (::strcmp(ptr->getHost(), hostname) == 0)
	    found = TRUE;
	else
	{
	    indx = ptr;
	    ptr = ptr->next;
	}
    }

    if (found)
    {
        if (indx == (GroupData*) 0)
	    tmplate = ptr->next;
	else
	    indx->next = ptr->next;

	delete ptr;
    }
}
	    
void TemplateObject::deleteAll (TemplateObject*& toDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void TemplateObject::deleteAll (TemplateObject*& toDelete)" << endl;
#endif

    if (toDelete)
    {
        GroupData* ptr = toDelete->tmplate;

	while (ptr)
	{
	    toDelete->tmplate = ptr->next;
	    delete ptr;
	    ptr = toDelete->tmplate;
	}

	toDelete->tmplate = (GroupData*) 0;
	delete toDelete;
	toDelete = (TemplateObject*) 0;
    }
}

Boolean TemplateObject::pack (Buffer& packInto) const
{
    Boolean res;

    if (tmplate == (GroupData*) 0)
        res = packInto.pack(-1);
    else
        res = packInto.pack(0) && tmplate->pack(packInto);

    return res;
}

Boolean TemplateObject::unpack (Buffer& unpackFrom)
{
    Boolean    res;
    int        ret  = 0;

    GroupData::deleteAll(tmplate);

    res = unpackFrom.unpack(ret);

    if (res)
    {
        if (ret == -1)
	    tmplate = (GroupData*) 0;
	else
	{
	    tmplate = new GroupData;
	    res = tmplate->unpack(unpackFrom);
	}
    }
    
    return res;
}

ostream& TemplateObject::print (ostream& strm) const
{
    GroupData* ptr = tmplate;
    
    while (ptr)
    {
	strm << "Template host : " << ptr->getHost() << "\n";
	ptr = ptr->next;
    }

    strm << "End of list\n";
    strm << "****\n";
    
    return strm;
}

ostream& operator<< (ostream& strm, const TemplateObject& to)
{
    return to.print(strm);
}
