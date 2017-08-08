/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QANBO.h,v 1.3 1996/01/02 13:15:52 nsmw Exp $
 */

#ifndef IMPLEMENTATION_BUFFER_QANBO_H_
#define IMPLEMENTATION_BUFFER_QANBO_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_BUFFER_H_
#   include <Implementation/Buffer.h>
#endif

class QANBOBufferImple : public BufferImple
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

    static  Resource *create();

    virtual Resource *control(const ClassName &ctrlInterName);

    static  QANBOBufferImple *castup(Resource *resource);
    virtual void             *castup(const ClassName &className) const;

protected:
    QANBOBufferImple();
    virtual ~QANBOBufferImple();

private:
    virtual void    sizeUp(size_t);
    virtual Boolean remaining(size_t);
    virtual void    alignedPackPointMove(size_t);
    virtual void    alignedUnpackPointMove(size_t);

    Boolean _good;

    char   *_packPoint;
    char   *_unpackPoint;

    size_t _bufferSize;
    char   *_buffer;
};

#endif
