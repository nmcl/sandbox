/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TemplateObject.cc,v
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef GROUPDATA_H_
#  include <RPC/ArjServers/GroupData.h>
#endif

#ifndef TEMPLATEOBJECT_H_
#  include "TemplateObject.h"
#endif

#ifndef REPDESC_H_
#  include "RepDesc.h"
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


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

    GroupData *ptr = tmplate, *indx = 0;

    while (ptr)
    {
        indx = ptr;
	ptr = ptr->next;
	delete indx;
    }
}

ReplicaDescriptor* TemplateObject::getTemplate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* TemplateObject::getTemplate ()" << endl;
#endif

    ReplicaDescriptor* x = new ReplicaDescriptor;
    *x = *tmplate;
    return x;
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

    tmplate->setHost((char*) hostname);
}

void TemplateObject::deleteTemplate (const char* hostname)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void TemplateObject::deleteTemplate ( " << hostname << " )" << endl;
#endif

    GroupData *ptr = tmplate, *indx = 0;
    Boolean found = FALSE;

    if (ptr == 0)
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
        if (indx == 0)
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

    GroupData *ptr = toDelete->tmplate, *indx = 0;

    while (ptr)
    {
        indx = ptr->next;
	delete ptr;
	ptr = indx;
    }

    toDelete->tmplate = 0;
    delete toDelete;
    toDelete = 0;
}

Boolean TemplateObject::pack (Buffer& packInto) const
{
    Boolean res;

    if (tmplate == 0)
        res = packInto.pack(-1);
    else
        res = packInto.pack(0) && tmplate->pack(packInto);

    return res;
}

Boolean TemplateObject::unpack (Buffer& unpackFrom)
{
    Boolean res;
    int ret = 0;
    GroupData *ptr = tmplate, *indx = 0;

    while (ptr)
    {
        indx = ptr->next;
	delete ptr;
	ptr = indx;
    }

    res = unpackFrom.unpack(ret);

    if (res)
    {
        if (ret == -1)
	    tmplate = 0;
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
