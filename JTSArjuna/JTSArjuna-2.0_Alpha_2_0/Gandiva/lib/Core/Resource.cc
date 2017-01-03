/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource.cc,v 1.7 1997/11/17 13:54:41 nmcl Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

Boolean Resource::_createdWithNew = FALSE;


Resource::Resource ()
                   : _refCount(0),
                     _heapAllocated(_createdWithNew)
{
    Resource::_createdWithNew = FALSE;
}

Resource::~Resource ()
{
    if ((_refCount != 0) && (_heapAllocated))
        cerr << "Resource::~Resource () - _refCount: " << _refCount
	     << endl;
}

/*
 * If allocated on the stack then we could simply ignore all
 * ref and unref requests. Currently we process them but
 * treat "error" conditions differently.
 */

void Resource::ref (Resource* resource)
{
    if (resource != NULL)
        resource->_refCount++;
}

Boolean Resource::unref (Resource* resource)
{
    if (resource != (Resource*) 0)
    {
        if (resource->_refCount > 0)
            resource->_refCount--;
        else
	    if (resource->_heapAllocated)
	        cerr << "Resource::unref - _refCount: 0" << endl;

        if ((resource->_refCount == 0) && (resource->_heapAllocated))
        {
            delete resource;
            resource = (Resource*) 0;
        }
    }

    if ((resource != (Resource*) 0) && (resource->_heapAllocated))
        return FALSE;
    else
        return TRUE;
}

void* Resource::operator new (size_t amount)
{
    Resource::_createdWithNew = TRUE;
    return ::new char[amount];
}

void Resource::operator delete (void* memPtr)
{
    // can we check reference count and issue a warning if none-zero?
    
    ::delete [] memPtr;
}
