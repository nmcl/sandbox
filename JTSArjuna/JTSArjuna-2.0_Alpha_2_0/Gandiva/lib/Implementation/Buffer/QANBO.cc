/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QANBO.cc,v 1.6 1997/05/20 16:13:34 nmcl Exp $
 */

#ifndef OS_STDDEF_H_
# include <os/stddef.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_STRING_H_
#  include <os/string.h>
#endif

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifdef WIN32

#ifndef OS_WINSOCK_H_
#   include <os/winsock.h>
#endif

#else

#ifndef OS_NETINET_IN_H_
#   include <os/netinet/in.h>
#endif

#endif

#ifndef GANDIVA_H_
#   include <Gandiva.h>
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

#ifndef IMPLEMENTATION_BUFFER_QANBO_H_
#   include <Implementation/Buffer/QANBO.h>
#endif

Boolean QANBOBufferImple::repack()
{
    if (_buffer != NULL)
    {
        _good      = TRUE;
        _packPoint = _buffer;

        return TRUE;
    }
    else
    {
        _good      = FALSE;
        _packPoint = _buffer;

        return FALSE;
    }
}

Boolean QANBOBufferImple::reunpack()
{
    if (_buffer != NULL)
    {
        _good        = TRUE;
        _unpackPoint = _buffer;

        return TRUE;
    }
    else
    {
        _good        = FALSE;
        _unpackPoint = _buffer;

        return FALSE;
    }
}

Boolean QANBOBufferImple::good() const
{
    return _good;
}

Boolean QANBOBufferImple::pack(char c)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(char));

        *_packPoint = c;

        alignedPackPointMove(sizeof(char));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(unsigned char uc)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(unsigned char));

        *_packPoint = uc;

        alignedPackPointMove(sizeof(unsigned char));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(int i)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(int));

        *((unsigned long*) _packPoint) = htonl(i);

        alignedPackPointMove(sizeof(int));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(unsigned int ui)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(unsigned int));

        *((unsigned long*) _packPoint) = htonl(ui);

        alignedPackPointMove(sizeof(unsigned int));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(long l)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(long));

        *((unsigned long*) _packPoint) = htonl(l);

        alignedPackPointMove(sizeof(long));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(unsigned long ul)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(unsigned long));

        *((unsigned long*) _packPoint) = htonl(ul);

        alignedPackPointMove(sizeof(unsigned long));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(short s)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(short));

        *((unsigned short*) _packPoint) = htons(s);

        alignedPackPointMove(sizeof(short));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(unsigned short us)
{
    if (_good && (_buffer != NULL))
    {
        sizeUp(sizeof(unsigned short));

        *((unsigned short*) _packPoint) = htons(us);

        alignedPackPointMove(sizeof(unsigned short));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::pack(float)
{
    return FALSE;
}

Boolean QANBOBufferImple::pack(double)
{
    return FALSE;
}

Boolean QANBOBufferImple::pack(const char *str)
{
    if (str == NULL)
        return pack(0);
    else
    {
         size_t sl = strlen(str) + 1;

         return pack(sl) && pack(str, sl);
    }
}

Boolean QANBOBufferImple::pack(const char *mem, size_t memlen)
{
    if (_good && (_buffer != NULL) && (mem != NULL) && (memlen > 0))
    {
        if (pack(memlen))
	{
            sizeUp(memlen);

            memcpy(_packPoint, mem, memlen);

            alignedPackPointMove(memlen);
            return TRUE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(char &c)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(char)))
    {
        c = *_unpackPoint;

        alignedUnpackPointMove(sizeof(char));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(unsigned char &uc)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(unsigned char)))
    {
        uc = *_unpackPoint;

        alignedUnpackPointMove(sizeof(unsigned char));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(int &i)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(int)))
    {
        i = (int) ntohl(*((unsigned long*) _unpackPoint));

        alignedUnpackPointMove(sizeof(int));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(unsigned int &ui)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(unsigned int)))
    {
        ui = (unsigned int) ntohl(*((unsigned long*) _unpackPoint));

        alignedUnpackPointMove(sizeof(unsigned int));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(long &l)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(long)))
    {
        l = ntohl(*((unsigned long*) _unpackPoint));

        alignedUnpackPointMove(sizeof(long));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(unsigned long &ul)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(unsigned long)))
    {
        ul = ntohl(*((unsigned long*) _unpackPoint));

        alignedUnpackPointMove(sizeof(unsigned long));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(short &s)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(short)))
    {
        s = ntohs(*((unsigned short*) _unpackPoint));

        alignedUnpackPointMove(sizeof(short));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(unsigned short &us)
{
    if (_good && (_buffer != NULL) && remaining(sizeof(unsigned short)))
    {
        us = ntohs(*((unsigned short*) _unpackPoint));

        alignedUnpackPointMove(sizeof(unsigned short));

        return TRUE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(float&)
{
    return FALSE;
}

Boolean QANBOBufferImple::unpack(double&)
{
    return FALSE;
}

Boolean QANBOBufferImple::unpack(char *&str)
{
    size_t sl;

    if (unpack(sl))
    {
        if ((sl == 0) && (str == NULL))
            return TRUE;
        else
        {
            if (str != NULL)
                ::delete [] str;

            str = new char[sl];

            if (unpack(str, sl, sl))
                return TRUE;
            else
	    {
                ::delete [] str;
                str = NULL;
                return FALSE;
	    }
        }
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::unpack(char *mem, size_t &memLen, size_t maxMemLen)
{
    if (_good && (_buffer != NULL) && (mem != NULL) && (maxMemLen > 0))
    {
        if (unpack(memLen))
	{
            if ((maxMemLen >= memLen) && remaining(memLen))
	    {
                memcpy(mem, _unpackPoint, memLen);

                alignedUnpackPointMove(memLen);

                return TRUE;
	    }
            else
                return FALSE;
        }
        else
            return FALSE;
    }
    else
        return FALSE;
}

Boolean QANBOBufferImple::setBuffer(char *buffer, size_t contentSize, size_t bufferSize)
{
    _good        = TRUE;

    if (_buffer != NULL)
        ::delete [] _buffer;

    _buffer      = buffer;
    _bufferSize  = bufferSize;
    _packPoint   = _buffer + contentSize;
    _unpackPoint = _buffer;

    return TRUE;
}

Boolean QANBOBufferImple::copySetBuffer(char *buffer, size_t contentSize, size_t bufferSize)
{
    _good        = TRUE;

    if (_buffer != NULL)
        ::delete [] _buffer;

    _buffer = new char[bufferSize];
    memcpy(_buffer, buffer, bufferSize);
    _bufferSize  = bufferSize;
    _packPoint   = _buffer + contentSize;
    _unpackPoint = _buffer;

    return TRUE;
}

Boolean QANBOBufferImple::getBuffer(char *&buffer, size_t &contentSize, size_t &bufferSize)
{
    buffer      = _buffer;
    contentSize = _packPoint - _buffer;
    bufferSize  = _bufferSize;
    
    return _good;
}

void QANBOBufferImple::sizeUp(size_t size)
{
    while ((size + (size_t) (_packPoint - _buffer)) > _bufferSize)
    {
        char *bufferNew;

        _bufferSize = 2 * _bufferSize;
        bufferNew = (char*) realloc(_buffer, _bufferSize);

        if (_buffer != bufferNew)
	{
            _packPoint   = bufferNew + (_packPoint - _buffer);
            _unpackPoint = bufferNew + (_unpackPoint - _buffer);
            _buffer      = bufferNew;
	}
    }
}

Boolean QANBOBufferImple::remaining(size_t size)
{
    return (_bufferSize >= (size + (size_t) (_unpackPoint - _buffer)));
}

void QANBOBufferImple::alignedPackPointMove(size_t size)
{
    int rem = ((int) (_packPoint + size)) % 4;

    if (rem == 0)
        _packPoint = _packPoint + size;
    else
        _packPoint = _packPoint + size + (4 - rem);
}

void QANBOBufferImple::alignedUnpackPointMove(size_t size)
{
    int rem = ((int) (_unpackPoint + size)) % 4;

    if (rem == 0)
        _unpackPoint = _unpackPoint + size;
    else
        _unpackPoint = _unpackPoint + size + (4 - rem);
}

const ClassName &QANBOBufferImple::name()
{
    return Gandiva::Implementation::Buffer::QANBO::name();
}

const ClassName &QANBOBufferImple::className() const
{
    return Gandiva::Implementation::Buffer::QANBO::name();
}

Resource *QANBOBufferImple::create()
{
    return new QANBOBufferImple;
}

Resource *QANBOBufferImple::control(const ClassName &)
{
    return NULL;
}

QANBOBufferImple::QANBOBufferImple()
{
    _good        = TRUE;
    _buffer      = new char[512];
    _bufferSize  = 512;
    _packPoint   = _buffer;
    _unpackPoint = _buffer;
}

QANBOBufferImple::~QANBOBufferImple()
{
    if (_buffer != NULL)
        ::delete [] _buffer;
}

QANBOBufferImple *QANBOBufferImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (QANBOBufferImple *) resource->castup(QANBOBufferImple::name());
    else
        return NULL;
}

void *QANBOBufferImple::castup(const ClassName &className) const
{
    if (className == QANBOBufferImple::name())
        return (void *) this;
    else
        return BufferImple::castup(className);
}
