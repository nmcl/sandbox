/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.h,v 1.3 1996/01/02 13:12:51 nsmw Exp $
 */

#ifndef IMPLEMENTATION_BUFFER_H_
#define IMPLEMENTATION_BUFFER_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

class BufferImple;

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

class Buffer;

#ifndef INTERFACE_BUFFER_H_
#   include <Interface/Buffer.h>
#endif

class BufferImple : virtual public Resource
{
public:
    virtual Boolean repack() = 0;
    virtual Boolean reunpack() = 0;

    virtual Boolean good() const = 0;

    virtual Boolean pack(char) = 0;
    virtual Boolean pack(unsigned char) = 0;
    virtual Boolean pack(int) = 0;
    virtual Boolean pack(unsigned int) = 0;
    virtual Boolean pack(long) = 0;
    virtual Boolean pack(unsigned long) = 0;
    virtual Boolean pack(short) = 0;
    virtual Boolean pack(unsigned short) = 0;
    virtual Boolean pack(float) = 0;
    virtual Boolean pack(double) = 0;
    virtual Boolean pack(const char*) = 0;
    virtual Boolean pack(const char*, size_t) = 0;
    
    virtual Boolean unpack(char&) = 0;
    virtual Boolean unpack(unsigned char&) = 0;
    virtual Boolean unpack(int&) = 0;
    virtual Boolean unpack(unsigned int&) = 0;
    virtual Boolean unpack(long&) = 0;
    virtual Boolean unpack(unsigned long&) = 0;
    virtual Boolean unpack(short&) = 0;
    virtual Boolean unpack(unsigned short&) = 0;
    virtual Boolean unpack(float&) = 0;
    virtual Boolean unpack(double&) = 0;
    virtual Boolean unpack(char*&) = 0;
    virtual Boolean unpack(char*, size_t&, size_t) = 0;

    virtual Boolean setBuffer(char*, size_t, size_t) = 0;
    virtual Boolean copySetBuffer(char*, size_t, size_t) = 0;
    virtual Boolean getBuffer(char*&, size_t&, size_t&) = 0;

    static  const ClassName &name();
    virtual const ClassName &className() const;

    virtual Resource *control(const ClassName &ctrlInterName) = 0;

    static  BufferImple *castup(Resource *resource);
    virtual void        *castup(const ClassName &className) const;

protected:
    BufferImple();
    virtual ~BufferImple();
};

#endif
