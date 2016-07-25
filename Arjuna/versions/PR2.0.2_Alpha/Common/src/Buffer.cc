/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.cc,v 1.6 1994/03/28 09:02:51 ngdp Exp $
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

static const char RCSid[] = "$Id: Buffer.cc,v 1.6 1994/03/28 09:02:51 ngdp Exp $";

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
    debug_stream << "Buffer::Buffer (" << chunkSize << ")" << endl;
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
    debug_stream << ", " << full << ", " << shouldDel << ")" << endl;
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
    debug_stream << "Buffer::Buffer (" << (void *)&copyFrom << ")" << endl;
#endif

    copybuff(copyFrom);
}
     
Buffer::~Buffer ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::~Buffer()" << endl;
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
 * These routines try to handle pointer aliasing in an attempt to avoid
 * sending pointed to objects more than once in a buffer. As pointers to
 * objects are added to the buffer they are looked up in a table and if
 * not found a TRUE value is returned indicating that the object should be 
 * packed. 
 * General comments on pointer packing.
 * Pointers are preceded in the buffer by a flag encoded as follows:
 * If the pointer is NULL then a value of 0 is packed
 * If the pointer is non-NULL and the pointer has not been seen before
 * a negative index is packed followed by the pointed to object otherwise the index
 * from the table is packed.
 * The unmarshalling code forces replacement of entries in the mapping table
 * as pointers are unpacked
 */

Boolean Buffer::shouldPack ( const void *ptr )
{
    Boolean doPack = FALSE;
    long index = 0;
	
    if (packPointers.addPtr(ptr, index))
    {
	pack(-index);			/* indicate new pointer */
	doPack = TRUE;
    }
    else
    {
	if (index < 0)
	    buffState = BUFFER_BAD;
	else
	    pack(index);
    }
    
    return doPack;
}

Boolean Buffer::saveUnpackedPtr ( const void *ptr, long& index )
{
    return unpackPointers.addPtr(ptr, index);
}

Boolean Buffer::unpackPtr ( const void*& ptr, long& index )
{
    Boolean unpackMe = FALSE;

    unpack(index);

    if (index >= 0)
    {
	ptr = unpackPointers[index];
    }
    else
	unpackMe = TRUE;

    return unpackMe;
}

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
    
    if (shouldPack(string))
    {
	/* shouldPack handles null pointers */
	
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
    
    return (realign(packPoint));
}

/* operation to pack a contiguous area of memory */

Boolean Buffer::pack ( const char *memory, const size_t amount )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (shouldPack(memory))
    {
	/* shouldPack has handled the possible null pointer */
	
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
    {
	/*
	 * We have packed this block before. shouldPack will
	 * have saved the index just add the amount here for
	 * unpack purposes
	 */
	
	if (!pack(amount))
	    return FALSE;
    }
    
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
    long slength;
    long index = 0;
    const void *p = 0;

    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (unpackPtr(p, index))
    {
	if (unpack(slength))
	{
	    string = new char[slength + 1];

	    if (string)
	    {
		memcpy(string, unpackPoint, slength);
		(string)[slength] = '\0';
		unpackPoint += slength;
		saveUnpackedPtr(string, index);
		return (realign(unpackPoint));
	    }
	}
    }
    else
    {	
	string = (char *)p;
	return TRUE;
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
    long index  =0;
    const void *p = 0;
    size_t memSize;

    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (unpackPtr(p,index))
    {
	if (unpack(memSize))
	{
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
		saveUnpackedPtr(memory, index);
		unpackPoint += amount;
		return (realign(unpackPoint));
	    }
	}
    }
    else
    {
	memory = (char *)p;
	if (unpack(amount))
	    return TRUE;
    }
    
    buffState = BUFFER_BAD;
    return (FALSE);
}

/*
 * Pack this buffer into another. Note that we don't call this simply
 * pack because it is the INVERSE of the normal pack. That is 'this' is
 * packed into the argument buffer, whereas normally (for standard types)
 * the argument is packed into 'this'
 */

Boolean Buffer::packInto ( Buffer& buff ) const
{
    Boolean res = FALSE;
    size_t len = length();
    
    if (buff.pack(packPoint - bufferStart) &&
	buff.pack(unpackPoint - bufferStart) &&
	(buff.pack(len)))
	if (len != 0)
	{
	    if (buff.pack(bufferStart, len) &&
		packPointers.pack(buff))
		res = unpackPointers.pack(buff);
	}
	else
	    res = TRUE;
    
    return res;
}

/*
 * Unpack this from the arg buffer. Obeys same rules as packInto above
 */

Boolean Buffer::unpackFrom ( Buffer& buff )
{
    size_t buffSize;
    char *tempBuffer = 0;
    Boolean res = FALSE;
    long packOffset = 0;
    long unpackOffset = 0;
    

    if (buff.unpack(packOffset) &&
	buff.unpack(unpackOffset) &&
	buff.unpack(buffSize))
    {
	if ((buffSize != 0) &&
	    (res = buff.unpack(tempBuffer, buffSize)))
	{
	    if (setBuffer(buffSize, tempBuffer) &&
		(packPointers.unpack(buff)))
		res = unpackPointers.unpack(buff);
	}
	if (res)
	{
	    packPoint = bufferStart + packOffset;
	    unpackPoint = bufferStart + unpackOffset;
	}
	
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
 * Pack this buffer into an RpcBuffer. Note this is almost the same as
 * packInto EXCEPT that it does not send the pointer alias info as that
 * would almost certainly be wrong in another address space.
 */

void Buffer::marshall ( RpcBuffer& buff ) const
{
    size_t len = length();
    
    if (buff.pack(packPoint - bufferStart) &&
	buff.pack(unpackPoint - bufferStart) &&
	(buff.pack(len)))
	if (len != 0)
	{
	    buff.pack(bufferStart, len);
	}
}

/*
 * Unpack this from the arg buffer.
 */

void Buffer::unmarshall ( RpcBuffer& buff )
{
    size_t buffSize;
    char *tempBuffer = 0;
    long packOffset = 0;
    long unpackOffset = 0;
    

    if (buff.unpack(packOffset) &&
	buff.unpack(unpackOffset) &&
	buff.unpack(buffSize))
    {
	if ((buffSize != 0) &&
	    (buff.unpack(tempBuffer, buffSize)))
	{
	    if (tempBuffer != 0)
	    {
		setBuffer(buffSize, tempBuffer);
	    }
	}

	packPoint = bufferStart + packOffset;
	unpackPoint = bufferStart + unpackOffset;
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

    packPointers = buff.packPointers;
    unpackPointers = buff.unpackPointers;
    
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

Boolean Buffer::realign ( char *& memPtr )
{
    if (((ptrdiff_t) memPtr) % ALIGNMENT)
    {
	size_t pad = (ALIGNMENT - (((ptrdiff_t) memPtr) % ALIGNMENT));

	if (sizeup(pad) == FALSE)
	{
	    buffState = BUFFER_BAD;
	    return FALSE;
	}

	/* sizeup has already zeroed the buffer if needed */
	memPtr += pad;

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
