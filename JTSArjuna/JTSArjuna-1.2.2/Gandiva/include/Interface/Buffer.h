/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.h,v 1.4 1996/04/22 09:19:31 nmcl Exp $
 */

#ifndef INTERFACE_BUFFER_H_
#define INTERFACE_BUFFER_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

class ClassName;

class ObjectName;

class BufferImple;

class Buffer : virtual public Resource
{
public:
    virtual Boolean repack();
    virtual Boolean reunpack();

    virtual Boolean good() const;

    virtual Boolean pack(char);
    virtual Boolean pack(unsigned char);
    virtual Boolean pack(int);
    virtual Boolean pack(unsigned int);
    virtual Boolean pack(long);
    virtual Boolean pack(unsigned long);
    virtual Boolean pack(short);
    virtual Boolean pack(unsigned short);
    virtual Boolean pack(float);
    virtual Boolean pack(double);
    virtual Boolean pack(const char*);
    virtual Boolean pack(const char*, size_t);
    
    virtual Boolean unpack(char&);
    virtual Boolean unpack(unsigned char&);
    virtual Boolean unpack(int&);
    virtual Boolean unpack(unsigned int&);
    virtual Boolean unpack(long&);
    virtual Boolean unpack(unsigned long&);
    virtual Boolean unpack(short&);
    virtual Boolean unpack(unsigned short&);
    virtual Boolean unpack(float&);
    virtual Boolean unpack(double&);
    virtual Boolean unpack(char*&);
    virtual Boolean unpack(char*, size_t&, size_t);
    
    virtual Boolean setBuffer(char*, size_t, size_t);
    virtual Boolean copySetBuffer(char*, size_t, size_t);
    virtual Boolean getBuffer(char*&, size_t&, size_t&);

    static  const ClassName &name();
    virtual const ClassName &className() const;
    virtual const ClassName &impleClassName() const;

    static  Buffer   *create(const ObjectName &objectName);
    static  Buffer   *create(const ClassName &bufferImpleName);

    virtual Resource *control(const ClassName &ctrlInterName);

    /*
     * Operator == ?
     */
    
    Buffer(const ObjectName &objectName);
    Buffer(const ClassName &bufferImpleName);
    Buffer(const Buffer &buffer);
    virtual ~Buffer();

    static  Buffer *castup(Resource *resource);
    virtual void   *castup(const ClassName &className) const;

protected:
    Buffer(BufferImple *_bufferImple);

private:
    BufferImple *_imple;
};

#endif
