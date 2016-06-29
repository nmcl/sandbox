/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcBuffer.cc,v 1.1 1997/06/09 19:52:06 nmcl Exp $
 */

static const char RCSid[] = "$Id: RpcBuffer.cc,v 1.1 1997/06/09 19:52:06 nmcl Exp $";

/*
 * Class to handle RPC system buffers. This version inherits from Arjuna 
 * Buffers directly
 *
 */

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

/*
 * PUBLIC operations
 */

/*
 * Constructors. 
 */

RpcBuffer::RpcBuffer ( )
                     : Buffer()
{
}

RpcBuffer::RpcBuffer ( Int32 initVal )
                     : Buffer()
{
    pack(initVal);
}

RpcBuffer::RpcBuffer ( const RpcBuffer& copyFrom )
                     : Buffer(copyFrom)
{
}

RpcBuffer::RpcBuffer ( const RpcBuffer& copyFrom, Int32 initVal )
                     : Buffer(copyFrom)
{
    pack(initVal);
}

RpcBuffer::~RpcBuffer ()
{
}

#ifdef HAVE_BOOLEAN_TYPE
RpcBuffer& RpcBuffer::operator<< ( const bool* b )
{
    if (shouldPack(b))
	pack(*b);

    return *this;
}
#endif

RpcBuffer& RpcBuffer::operator<< ( const char* s )
{
    packMappedString(s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const short* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned short* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const int* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned int* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const long* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned long* s )
{
    if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const float* s )
{
     if (shouldPack(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const double* s )
{
    if (shouldPack(s))
	pack(*s);
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( char* &s )
{
    unpack(s);
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( short* &s )
{
    operator>>((unsigned short *&)s);
    return *this;
}

#ifdef HAVE_BOOLEAN_TYPE
RpcBuffer& RpcBuffer::operator>> ( bool* &s )
{
    Int32 index = 0;
    const void *p = 0;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new bool;
	unpack(*s);
	saveUnpackedPtr(s, index);
    }
    else
	s = (bool *)p;

    return *this;
}
#endif

RpcBuffer& RpcBuffer::operator>> ( unsigned short* &s )
{
    Int32 index = 0;
    const void *p = 0;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned short;
	unpack(*s);
	saveUnpackedPtr(s, index);
    }
    else
	s = (unsigned short *)p;
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( int* &s )
{
    operator>>((unsigned int *&)s);
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( unsigned int* &s )
{
    Int32 index = 0;
    const void *p = 0;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned int;
	unpack(*s);
	saveUnpackedPtr(s, index);
    }
    else
	s = (unsigned int *)p;
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( long* &s )
{
    operator>>((unsigned long *&)s);
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( unsigned long* &s )
{
    Int32 index = 0;
    const void *p = 0;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned long;
	unpack(*s);
	saveUnpackedPtr(s,index);
    }
    else
	s = (unsigned long *)p;
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( float* &s )
{
    Int32 index = 0;
    const void *p = 0;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new float;
	unpack(*s);
	saveUnpackedPtr(s,index);
    }
    else
	s = (float *)p;

    return *this;
}

#ifdef NO_INLINES
#  define RPCBUFFER_CC_
#  include <StubGen/RpcBuffer.n>
#  undef RPCBUFFER_CC_
#endif
