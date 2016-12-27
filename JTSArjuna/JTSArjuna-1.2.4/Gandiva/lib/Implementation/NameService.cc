/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameService.cc,v 1.3 1996/01/02 13:15:45 nsmw Exp $
 */

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

const ClassName &NameServiceImple::name()
{
    return Gandiva::Implementation::NameService::name();
}

const ClassName &NameServiceImple::className() const
{
    return Gandiva::Implementation::NameService::name();
}

NameServiceImple *NameServiceImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (NameServiceImple *) resource->castup(NameServiceImple::name());
    else
        return NULL;
}

void *NameServiceImple::castup(const ClassName &className) const
{
    if (className == NameServiceImple::name())
        return (void *) this;
    else
        return NULL;
}

NameServiceImple::NameServiceImple()
{
}

NameServiceImple::~NameServiceImple()
{
}
