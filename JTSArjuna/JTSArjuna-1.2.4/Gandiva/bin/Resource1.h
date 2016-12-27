/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource1.h,v 1.1 1997/06/20 11:42:30 nsmw Exp $
 */

#ifndef RESOURCE1_H_
#define RESOURCE1_H_

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

class ClassName;

class Resource1 : public Resource
{
public:
    static const ClassName &name();
    const ClassName &className() const;

    Resource1 *castup(Resource *resource);
    virtual void *castup(const ClassName&) const;
    
    Resource1();
    virtual ~Resource1();

private:
    static ClassName *_className; 
};

#endif
