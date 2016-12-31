/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource.h,v 1.6 1997/11/17 13:54:32 nmcl Exp $
 */

#ifndef CORE_RESOURCE_H_
#define CORE_RESOURCE_H_

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

class ClassName;


class Resource
{
public:
    static void    ref   (Resource* resource);
    static Boolean unref (Resource* resource);

    void* operator new (size_t amount);
    void operator delete (void* memPtr);

    virtual void* castup (const ClassName&) const = 0;
    
protected:
    Resource ();
    virtual ~Resource ();

private:
    unsigned int   _refCount;
    Boolean        _heapAllocated;

    static Boolean _createdWithNew;
};

#endif
