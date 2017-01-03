/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.cc,v 1.3 1996/01/02 13:15:43 nsmw Exp $
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

#ifndef IMPLEMENTATION_BUFFER_H_
#   include <Implementation/Buffer.h>
#endif

const ClassName &BufferImple::name()
{
    return Gandiva::Implementation::Buffer::name();
}

const ClassName &BufferImple::className() const
{
    return Gandiva::Implementation::Buffer::name();
}

BufferImple *BufferImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (BufferImple *) resource->castup(BufferImple::name());
    else
        return NULL;
}

void *BufferImple::castup(const ClassName &className) const
{
    if (className == BufferImple::name())
        return (void *) this;
    else
        return NULL;
}

BufferImple::BufferImple()
{
}

BufferImple::~BufferImple()
{
}
