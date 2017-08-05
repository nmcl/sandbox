/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Resource1.cc,v 1.1 1997/06/20 11:42:29 nsmw Exp $
 */

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef RESOURCE1_H_
#   include "Resource1.h"
#endif

const ClassName &Resource1::name()
{
    if (_className == NULL)
        _className = new ClassName("#Resource1");

    return *_className;
}

const ClassName &Resource1::className() const
{
    if (_className == NULL)
        _className = new ClassName("#Resource1");

    return *_className;
}

Resource1 *Resource1::castup(Resource *resource)
{
    if (resource != NULL)
        return (Resource1 *) resource->castup(Resource1::name());
    else
        return NULL;
}

void *Resource1::castup(const ClassName &className) const
{
    if (className == Resource1::name())
        return (void *) this;
    else
        return NULL;
}

Resource1::Resource1()
{
    cout << "Resource1(): " << this << endl;
}

Resource1::~Resource1()
{
    cout << "~Resource1(): " << this << endl;
}

ClassName *Resource1::_className = NULL;
