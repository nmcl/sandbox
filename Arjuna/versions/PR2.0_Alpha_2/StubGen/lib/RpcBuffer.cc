/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcBuffer.cc,v 1.1 1993/11/03 14:46:38 nmcl Exp $
 */

static const char RCSid[] = "$Id: RpcBuffer.cc,v 1.1 1993/11/03 14:46:38 nmcl Exp $";

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
 * Constructors. NOTE these do not explicitly clear the map array
 * but rely on mapPtr doing it on demand
 */

RpcBuffer::RpcBuffer ( )
                     : Buffer(),
		       mapEntries(0)
{
}

RpcBuffer::RpcBuffer ( long initVal )
                     : Buffer(),
		       mapEntries(0)
{
    pack(initVal);
}

RpcBuffer::RpcBuffer ( const RpcBuffer& copyFrom )
                     : Buffer(copyFrom),
		       mapEntries(0)
{
}

RpcBuffer::RpcBuffer ( const RpcBuffer& copyFrom, long initVal )
                     : Buffer(copyFrom),
		       mapEntries(0)
{
    pack(initVal);
}

RpcBuffer::~RpcBuffer ()
{
}

/*
 * These routines try to handle pointer aliasing in an attempt to avoid
 * sending pointed to objects more than once in a buffer. As pointers to
 * objects are added to the buffer they are looked up in a table and if
 * not found a TRUE value is returned indicating that the object should be 
 * packed. 
 * General comments on pointer packing.
 * Pointers are preceded in the buffer by a flag encoded as follows:
 * If the pointer is NULL then a value of 0 is packed
 * If the pointer is non-NULL and the pointer has not been seen before
 * a -1 is packed followed by the pointed to object otherwise the index
 * from the table is packed.
 */

Boolean RpcBuffer::addPtr ( const void *ptr, long& index )
{
    Boolean addOk = FALSE;
    
    index = mapPtr(ptr);
    
    if (ptr != 0)
    {
	if (ptrMapping[index] == 0)
	{
	    ptrMapping[index] = ptr;	/* remember it */
	    mapEntries++;
	    addOk = TRUE;
	}
    }
    
    return addOk;
}

/*
 * Clear the mapping table. If the argument is TRUE we zero the whole
 * thing, otherwise we only reset the number of entries to 0 and rely on
 * mapPtr to clear entries lazily as it needs them.
 */

void RpcBuffer::clearMap ( Boolean clearAll )
{
    if (clearAll)
    {
	for (int i = 0; i < MAP_TBL_SIZE; i++)
	    ptrMapping[i] = 0;
    }

    mapEntries = 0;
 }

Boolean RpcBuffer::packPtr ( const void *ptr )
{
    Boolean do_pack = FALSE;
    
    if (ptr != 0)
    {
	long index;
	
	if (addPtr(ptr, index))
	{
	    pack(-1);			/* indicate new pointer */
	    do_pack = TRUE;
	}
	else
	    pack(index);
    }
    else
	pack(0);
    
    return do_pack;
}

#ifdef _AIX
Boolean RpcBuffer::unpackPtr ( void*& ptr, long& index )
#else
Boolean RpcBuffer::unpackPtr ( const void*& ptr, long& index )
#endif
{
    Boolean unpackMe = FALSE;

    ptr = 0;

    unpack(index);

    if (index >= 0)
    {
	if (index > 0)
	    ptr = ptrMapping[index];
    }
    else
	unpackMe = TRUE;

    return unpackMe;
}

RpcBuffer& RpcBuffer::operator<< ( const char* s )
{
    if (packPtr(s))
	pack(s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const short* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned short* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const int* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned int* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const long* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const unsigned long* s )
{
    if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const float* s )
{
     if (packPtr(s))
	pack(*s);

    return *this;
}

RpcBuffer& RpcBuffer::operator<< (  const double* s )
{
    if (packPtr(s))
	pack(*s);
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( char* &s )
{
    long index;
    void *p;

    if (unpackPtr(p, index))
    {
	unpack(s);
	ptrMapping[index] = s;
    }
    else
	s = (char *)p;
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( short* &s )
{
    operator>>((unsigned short *&)s);
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( unsigned short* &s )
{
    long index;
    void *p;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned short;
	unpack(*s);
	ptrMapping[index] = s;
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
    long index;
    void *p;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned int;
	unpack(*s);
	ptrMapping[index] = s;
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
    long index;
    void *p;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new unsigned long;
	unpack(*s);
	ptrMapping[index] = s;
    }
    else
	s = (unsigned long *)p;
    
    return *this;
}

RpcBuffer& RpcBuffer::operator>> ( float* &s )
{
    long index;
    void *p;

    if (unpackPtr(p, index))
    {
	if (s == 0)
	    s = new float;
	unpack(*s);
	ptrMapping[index] = s;
    }
    else
	s = (float *)p;

    return *this;
}

/*
 * returns index of slot containing pointer or index of a nil slot
 * Set nil entry explicitly if pointer not found
 */

long RpcBuffer::mapPtr ( const void *p )
{
    long i = 0;

    if (p != 0)
    {
	i++;				/* slot 0 is always empty */
	while (i <= mapEntries)
	{
	    if (ptrMapping[i] == p)
		return i;
	    i++;
	}
    }

    ptrMapping[i] = 0;
    return i;
}

#ifdef NO_INLINES
#  define RPCBUFFER_CC_
#  include <StubGen/RpcBuffer.n>
#  undef RPCBUFFER_CC_
#endif
