/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.cc,v 1.35 1993/09/22 11:35:49 ngdp Exp $
 */

/*
 *
 * Class that implements the marshalling operations for the primitive C types
 * supported by C++ (with certain obvious exceptions).
 * The internal char * buffer is managed internally and will be freed
 * when the Buffer instance is destroyed REGARDLESS of where it was created.
 *
 */

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef VALUES_H_
#  include <System/values.h>
#endif

#ifndef CTYPE_H_
#  include <System/ctype.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef RPCBUFFER_H_
#  include <StubGen/RpcBuffer.h>
#endif

static const int ALIGNMENT = 4;

static const char RCSid[] = "$Id: Buffer.cc,v 1.35 1993/09/22 11:35:49 ngdp Exp $";

/* 
 * Public constructors and destructors
 */

/*
 * The following constructor is provided for creating an empty Buffer.
 * The argument may be used to define the amount of memory allocated
 * each time extra memory is required - it defaults to the value
 * in Buffer.h 
 */

Buffer::Buffer ( size_t buffSize )
                 : buffState(BUFFER_GOOD),
		   bufferStart(0),
		   bufferEnd(0),
		   packPoint(0),
		   unpackPoint(0),
		   chunkSize(buffSize),
		   shouldDelete(TRUE)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << chunkSize << ")\n" << flush;
#endif
}

/*
 * The following constructor is provided for creating a Buffer to manage
 * an existing block of memory, or for creating a Buffer when the size of
 * the Buffer is known beforehand. If the boolean argument is TRUE then
 * the char* buffer is assumed to already contain data. If FALSE then
 * the size_t argument defines the size of the EMPTY char* buffer.
 */

Buffer::Buffer ( size_t buffSize, char *buffer, Boolean full,
		 Boolean shouldDel ) 
               : buffState(BUFFER_GOOD),
		 bufferStart(buffer),
		 bufferEnd(buffer+buffSize),
		 packPoint(full ? buffer+buffSize : buffer),
		 unpackPoint(buffer),
		 chunkSize(DEFAULT_CHUNK_SIZE),
		 shouldDelete(shouldDel)
                                                 
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << buffSize << ", " << (void *) buffer;
    debug_stream << ", " << full << ", " << shouldDel << ")\n" << flush;
#endif
}

/*
 * Initialise one buffer from another
 */

Buffer::Buffer ( const Buffer& copyFrom )                 
               : buffState(BUFFER_GOOD),
		 bufferStart(0),
		 bufferEnd(0),
		 packPoint(0),
		 unpackPoint(0),
		 chunkSize(0),
		 shouldDelete(TRUE)
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << (void *)&copyFrom << ")\n" << flush;
#endif

    copybuff(copyFrom);
}
     
Buffer::~Buffer ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::~Buffer()\n" << flush;
#endif

    if (shouldDelete && (bufferStart != 0))
    {
#ifndef __GNUG__
	delete [] bufferStart;
#else
	delete bufferStart;
#endif
	bufferStart = 0;
    }
}

/* 
 * Public non-virtual member functions and operators
 */

/*
 * Pack integral types - assumed pack point already aligned and
 * that realign will re-establish alignment (here should be noop)
 */

Boolean Buffer::pack ( const unsigned long l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    /* Ensure room exists */

    if (sizeup(sizeof(unsigned long)) == FALSE)
    {
	buffState = BUFFER_BAD;
	return (FALSE);
    }

    /* and then pack */

    *((unsigned long *) packPoint) = htonl(l);
    packPoint += sizeof(unsigned long);
    
    return (realign(packPoint));
}

Boolean Buffer::pack ( const double d )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX)
    const long *long_p = (const long *) &d;

    return ((pack(*long_p) && pack(*(++long_p))) ? TRUE : FALSE);
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

/* a null terminated string */

Boolean Buffer::pack ( const char *string )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (string)
    {
	size_t slength = strlen(string);

	if (pack(slength) && sizeup(slength))
	{
	    if (slength > 0)
	    {
		memcpy(packPoint, string, slength);
		packPoint += slength;
	    }

	}
	else
	    return FALSE;		/* buffer already bad */	    
    }
    else
	pack(-1);

    return (realign(packPoint));
}

/* operation to pack a contiguous area of memory */

Boolean Buffer::pack ( const char *memory, const size_t amount )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (memory)
    {
	if (pack(amount) && sizeup(amount))
	{
	    if (amount > 0)
	    {
		memcpy(packPoint, memory, (int) amount);
		packPoint += amount;
	    }
	}
	else
	    return FALSE;		/* buffer already bad */	    
    }
    else
	pack (-1);

    return (realign(packPoint));
}

Boolean Buffer::unpack ( unsigned char& c )
{
    unsigned long l;
    Boolean res = unpack(l);

    c = (unsigned char)l;

    return res;
}

Boolean Buffer::unpack ( unsigned short& s )
{
    unsigned long l;
    Boolean res = unpack(l);

    s = (unsigned short)l;

    return res;
}

Boolean Buffer::unpack ( unsigned int& i )
{
    unsigned long l;
    Boolean res = unpack(l);

    i = (unsigned int)l;

    return res;

}

/*
 * Unpack integral quantity - assume unpackPoint is aligned (ala pack)
 * and let realign maintain that
 */

Boolean Buffer::unpack ( unsigned long& l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if ((unpackPoint + sizeof(unsigned long)) <= packPoint)
    {
	l = ntohl(*((long *) unpackPoint));
	unpackPoint += sizeof(unsigned long);
	return (realign(unpackPoint));
    }

    buffState = BUFFER_BAD;
    return (FALSE);
}

Boolean Buffer::unpack ( float& f )
{
    double d;
    Boolean res = unpack(d);

    f = d;

    return res;
}

Boolean Buffer::unpack ( double& d )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX)
    long *long_p = (long *) &d;

    return ((unpack(*long_p) && unpack(*(++long_p))) ? TRUE : FALSE);
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

/* 
 * Unpack a null terminated string. 
 * Always unpacks into a new area of memory
 */

Boolean Buffer::unpack ( char *& string )
{
    size_t slength;

    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (unpack(slength))
    {
	/* If there is memory already and its not to be used again */
	
	if (slength == -1)
	{
	    string = 0;
	    return TRUE;
	}

	string = new char[slength + 1];

	if (string)
	{
	    memcpy(string, unpackPoint, slength);
	    (string)[slength] = '\0';
	    unpackPoint += slength;
	    return (realign(unpackPoint));
	}
    }

    buffState = BUFFER_BAD;
    return (FALSE);
}

/*
 * Operation to unpack a contiguous block of memory.
 * The long argument is set to the size unpacked.
 */

Boolean Buffer::unpack (char *& memory, size_t& amount )
{

    size_t memSize;

    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (unpack(memSize))
    {
	if (memSize == -1)
	{
	    memory = 0;
	    amount = 0;
	    return TRUE;
	}

	if (memory == 0)
	{
	    memory = new char[memSize];
	    amount = memSize;
	}
	else
	{
	    /* alocated memory large enough? */
	    if (memSize > amount)
	    {
		amount = memSize;

		/* always (un)pack ints etc as unsigned longs */

		unpackPoint -= sizeof(unsigned long);
		return (FALSE);
	    }
	}

	if (memory)
	{
	    memcpy(memory, unpackPoint, (int) amount);
	    unpackPoint += amount;
	    return (realign(unpackPoint));
	}
    }

    buffState = BUFFER_BAD;
    return (FALSE);
}

/*
 * Pack this buffer into another. Note that we don't call this simply
 * pack because it is the INVERSE of the normal pack. That is 'this' is
 * packed into the argument buffer, whereas normally (for standadr types)
 * the argument is packed into 'this'
 */

Boolean Buffer::packInto ( Buffer& buff ) const
{
    Boolean res;
    size_t len = length();
    
    if ((res = buff.pack(len)) &&
	(len != 0))
	res = buff.pack(bufferStart, len);
    
    return res;
}

/*
 * Unpack this from the arg buffer. Obeys same rules as packInto above
 */

Boolean Buffer::unpackFrom ( Buffer& buff )
{
    size_t buffSize;
    char *tempBuffer = 0;
    Boolean res;

    if ((res = buff.unpack(buffSize)) && (buffSize != 0))
    {
	if ((res = buff.unpack(tempBuffer, buffSize)) &&
	    (tempBuffer != 0))
	    res = setBuffer(buffSize, tempBuffer);
    }

    return res;
}

/*
 * Concatenate the unread portion of the arg buffer into this buffer
 */

Boolean Buffer::appendUnused ( const Buffer& from )
{
    size_t lenToPack = from.bufferEnd - from.unpackPoint;
    
    return pack(from.unpackPoint, lenToPack);
    
}

/*
 * Pack this buffer into an RpcBuffer.
 */

void Buffer::marshall ( RpcBuffer& buff ) const
{
    size_t len = length();
    
    if (buff.pack(len) && (len != 0))
	buff.pack(bufferStart, len);
}

/*
 * Unpack this from the arg buffer.
 */

void Buffer::unmarshall ( RpcBuffer& buff )
{
    size_t buffSize;
    char *tempBuffer = 0;
    Boolean res;

    if ((res = buff.unpack(buffSize)) && (buffSize != 0))
    {
	if ((res = buff.unpack(tempBuffer, buffSize)) &&
	    (tempBuffer != 0))
	    res = setBuffer(buffSize, tempBuffer);
    }
}

Buffer& Buffer::operator= ( const Buffer& buff )
{
    copybuff(buff);
    return *this;
}

Buffer& Buffer::operator+= ( const Buffer& buff )
{
    if (buffState != BUFFER_GOOD || buff.readState() != BUFFER_GOOD)
	return (*this);

    if (buff.length() > 0)
    {
	if (sizeup(buff.length()))
	{
	    memcpy(packPoint, buff.bufferStart, (int) buff.length());
	    packPoint += buff.length();
	    realign(packPoint);
	}
	else
	    buffState = BUFFER_BAD;
    }
    return *this;
}

Boolean Buffer::operator== (const Buffer& buff) const
{
    if (length() == buff.length())
	return (Boolean) (memcmp(buffer(), buff.buffer(), length()) == 0);
    else
	return FALSE;
}

/*
 * Public virtual functions
 */

ostream& Buffer::print ( ostream& strm ) const
{
    strm << "<" << buffState << "><"
         << (void *) bufferStart << "->" << (void *) bufferEnd
	 << "><"
	 << (void *) packPoint << "," << (void *) unpackPoint << ">(";
    
    if ((bufferStart != 0) && (packPoint != 0))
    {
	for (char *i = bufferStart; i < packPoint; i++)
	{
	    strm << (int) *i;
	    if (isprint(*i))
		strm << " [" << *i << "] ";
	    else
		strm << " {?} " ;
	}
    }
    strm << ")";

    return strm;
}

Boolean Buffer::setBuffer ( size_t buffSize, char *buffer, 
			     Boolean full,
			     Boolean shouldDel )
{
    if ((bufferStart != NULL) && shouldDelete)
	delete bufferStart;
    
    shouldDelete = shouldDel;
    bufferStart  = buffer;
    bufferEnd    = buffer+buffSize;
    packPoint    = (full ? bufferEnd : bufferStart);
    unpackPoint  = buffer;
    
    return TRUE;
}

/*
 * Private non-virtual functions
 */

/*
 * Make a copy of the argument buffer in the current one
 */

void Buffer::copybuff ( const Buffer& buff )
{
    if (buffState != BUFFER_GOOD || buff.readState() != BUFFER_GOOD)
	return;

    /*
     * copy over parameters first
     */

    chunkSize = buff.chunkSize;
   
    /*
     * Only copy buffer if actually necessary
     */

    if (buff.length() > 0)
    {
	if (sizeup(buff.length()))
	{
	    memcpy(bufferStart, buff.bufferStart, (unsigned int) buff.length());
	    packPoint = bufferStart + buff.length();

	    if (buff.unpackPoint > buff.bufferStart)
		unpackPoint = bufferStart +
		    (buff.unpackPoint - buff.bufferStart);
	    else
		unpackPoint = bufferStart;
	}
	else
	    buffState = BUFFER_BAD;
    }
    else
	bufferStart = bufferEnd = packPoint = unpackPoint = (char *) 0;
}

/*
 * Realign the pack or unpack points 
 * to the alignment boundary dictated by ALIGNMENT
 */

Boolean Buffer::realign ( char *& mem_ptr )
{
    if (((ptrdiff_t) mem_ptr) % ALIGNMENT)
    {
	size_t pad = (ALIGNMENT - (((ptrdiff_t) mem_ptr) % ALIGNMENT));

	if (sizeup(pad) == FALSE)
	{
	    buffState = BUFFER_BAD;
	    return FALSE;
	}

	/* sizeup has already zeroed the buffer if needed */
	mem_ptr += pad;

	return (TRUE);
    }
    else
	return (TRUE);
}

Boolean Buffer::sizeup ( size_t extra )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if ((packPoint + extra) > bufferEnd)
    {
	size_t newSize;
	size_t oldSize = length();
	char *oldStart = bufferStart;

	/* set new size */
	newSize = oldSize + extra;

	/* round up to nearest multiple of chunkSize */
	if (newSize % chunkSize)
	    newSize += chunkSize - (newSize % chunkSize);

	if (bufferStart == 0)
	    bufferStart = packPoint = unpackPoint = new char[newSize];
	else
	{
	    /* realloc */

	    bufferStart = new char[newSize];
	    if (bufferStart)
	    {
		memcpy(bufferStart, oldStart, oldSize);
		if (shouldDelete)
#ifndef __GNUG__
		    delete [] oldStart;
#else
		    delete oldStart;
#endif		
	    }
	}

	shouldDelete = TRUE;

	if (bufferStart == 0)
	{
	    bufferStart = oldStart;

	    error_stream << WARNING << "Buffer::sizeup malloc/realloc failure\n";
	    buffState = BUFFER_BAD;
	    return (FALSE);
	}
	else
	{
	    bufferEnd = bufferStart + newSize;

	    if (oldStart)
	    {
		/* buffer start MUST have changed by this point. So.. */
		packPoint = bufferStart + (packPoint - oldStart);
		unpackPoint = bufferStart + (unpackPoint - oldStart);
	    }
	    memset(packPoint, '\0', bufferEnd - packPoint);
	}
    }

    return (TRUE);
}

#ifdef NO_INLINES
#  define BUFFER_CC_
#  include "Common/Buffer.n"
#  undef BUFFER_CC_
#endif
