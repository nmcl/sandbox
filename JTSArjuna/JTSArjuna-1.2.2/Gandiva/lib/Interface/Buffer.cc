/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.cc,v 1.3 1996/01/02 13:34:24 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_BUFFER_H_
#   include <Implementation/Buffer.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#   include <Interface/Buffer.h>
#endif

Boolean Buffer::repack()
{
    if (_imple != NULL)
        return _imple->repack();
    else
        return FALSE;
}

Boolean Buffer::reunpack()
{
    if (_imple != NULL)
        return _imple->reunpack();
    else
        return FALSE;
}

Boolean Buffer::good() const
{
    if (_imple != NULL)
        return _imple->good();
    else
        return FALSE;
}

Boolean Buffer::pack(char c)
{
    if (_imple != NULL)
        return _imple->pack(c);
    else
        return FALSE;
}

Boolean Buffer::pack(unsigned char uc)
{
    if (_imple != NULL)
        return _imple->pack(uc);
    else
        return FALSE;
}

Boolean Buffer::pack(int i)
{
    if (_imple != NULL)
        return _imple->pack(i);
    else
        return FALSE;
}

Boolean Buffer::pack(unsigned int ui)
{
    if (_imple != NULL)
        return _imple->pack(ui);
    else
        return FALSE;
}

Boolean Buffer::pack(long l)
{
    if (_imple != NULL)
        return _imple->pack(l);
    else
        return FALSE;
}

Boolean Buffer::pack(unsigned long ul)
{
    if (_imple != NULL)
        return _imple->pack(ul);
    else
        return FALSE;
}

Boolean Buffer::pack(short s)
{
    if (_imple != NULL)
        return _imple->pack(s);
    else
        return FALSE;
}

Boolean Buffer::pack(unsigned short us)
{
    if (_imple != NULL)
        return _imple->pack(us);
    else
        return FALSE;
}

Boolean Buffer::pack(float f)
{
    if (_imple != NULL)
        return _imple->pack(f);
    else
        return FALSE;
}

Boolean Buffer::pack(double d)
{
    if (_imple != NULL)
        return _imple->pack(d);
    else
        return FALSE;
}

Boolean Buffer::pack(const char *str)
{
    if (_imple != NULL)
        return _imple->pack(str);
    else
        return FALSE;
}

Boolean Buffer::pack(const char *str, size_t st)
{
    if (_imple != NULL)
        return _imple->pack(str, st);
    else
        return FALSE;
}

Boolean Buffer::unpack(char &c)
{
    if (_imple != NULL)
        return _imple->unpack(c);
    else
        return FALSE;
}

Boolean Buffer::unpack(unsigned char &uc)
{
    if (_imple != NULL)
        return _imple->unpack(uc);
    else
        return FALSE;
}

Boolean Buffer::unpack(int &i)
{
    if (_imple != NULL)
        return _imple->unpack(i);
    else
        return FALSE;
}

Boolean Buffer::unpack(unsigned int &ui)
{
    if (_imple != NULL)
        return _imple->unpack(ui);
    else
        return FALSE;
}

Boolean Buffer::unpack(long &l)
{
    if (_imple != NULL)
        return _imple->unpack(l);
    else
        return FALSE;
}

Boolean Buffer::unpack(unsigned long &ul)
{
    if (_imple != NULL)
        return _imple->unpack(ul);
    else
        return FALSE;
}

Boolean Buffer::unpack(short &s)
{
    if (_imple != NULL)
        return _imple->unpack(s);
    else
        return FALSE;
}

Boolean Buffer::unpack(unsigned short &us)
{
    if (_imple != NULL)
        return _imple->unpack(us);
    else
        return FALSE;
}

Boolean Buffer::unpack(float &f)
{
    if (_imple != NULL)
        return _imple->unpack(f);
    else
        return FALSE;
}

Boolean Buffer::unpack(double &d)
{
    if (_imple != NULL)
        return _imple->unpack(d);
    else
        return FALSE;
}

Boolean Buffer::unpack(char *&d)
{
    if (_imple != NULL)
        return _imple->unpack(d);
    else
        return FALSE;
}

Boolean Buffer::unpack(char *bytes, size_t &bytesNumber, size_t bytesMax)
{
    if (_imple != NULL)
        return _imple->unpack(bytes, bytesNumber, bytesMax);
    else
        return FALSE;
}
    
Boolean Buffer::setBuffer(char *buff, size_t contentSize, size_t bufferSize)
{
    if (_imple != NULL)
        return _imple->setBuffer(buff, contentSize, bufferSize);
    else
        return FALSE;
}

Boolean Buffer::copySetBuffer(char *buff, size_t contentSize, size_t bufferSize)
{
    if (_imple != NULL)
        return _imple->copySetBuffer(buff, contentSize, bufferSize);
    else
        return FALSE;
}

Boolean Buffer::getBuffer(char *&buff, size_t &contentSize, size_t &bufferSize)
{
    if (_imple != NULL)
        return _imple->getBuffer(buff, contentSize, bufferSize);
    else
        return FALSE;
}

const ClassName &Buffer::name()
{
    return Gandiva::Interface::Buffer::name();
}

const ClassName &Buffer::className() const
{
    return Gandiva::Interface::Buffer::name();
}

const ClassName &Buffer::impleClassName() const
{
    if (_imple != NULL)
        return _imple->className();
    else
        return ClassName::invalid();
}

Buffer *Buffer::create(const ClassName &bufferImpleName)
{
    Buffer    *res    = NULL;
    Inventory *invent = Inventory::inventory();

    if (invent != NULL)
    {
        Resource::ref(invent);

        BufferImple *bufferImple = BufferImple::castup(invent->createVoid(bufferImpleName));

        if (bufferImple != NULL)
            res = new Buffer(bufferImple);

        Resource::unref(invent);
    }

    return res;
}

Resource *Buffer::control(const ClassName &ctrlInterName)
{
    if (_imple != NULL)
        return _imple->control(ctrlInterName);
    else
        return NULL;
}

Buffer *Buffer::castup(Resource *resource)
{
    if (resource != NULL)
        return (Buffer *) resource->castup(Buffer::name());
    else
        return NULL;
}

void *Buffer::castup(const ClassName &className) const
{
    if (className == Buffer::name())
        return (void *) this;
    else
        return NULL;
}

Buffer::Buffer(const ClassName &bufferImpleName)
{
    Inventory *invent = Inventory::inventory();

    _imple = NULL;

    if (invent != NULL)
    {
        Resource::ref(invent);

        _imple = BufferImple::castup(invent->createVoid(bufferImpleName));
        Resource::ref(_imple);

        Resource::unref(invent);
    }
}

Buffer::Buffer(const Buffer &buffer)
{
    _imple = buffer._imple;
    Resource::ref(_imple);
}

Buffer::Buffer(BufferImple *bufferImple)
{
    _imple = bufferImple;
    Resource::ref(_imple);
}

Buffer::~Buffer()
{
    Resource::unref(_imple);
}
