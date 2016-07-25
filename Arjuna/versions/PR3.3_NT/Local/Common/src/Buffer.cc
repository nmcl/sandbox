/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Buffer.cc,v 1.22 1996/04/15 10:59:15 nmcl Exp $
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

#ifndef WIN32
#  ifndef NETINET_IN_H_
#    include <System/netinet/in.h>
#  endif
#else
#  include <System/winsock.h>
#endif

#ifndef WIN32
#  ifndef VALUES_H_
#    include <System/values.h>
#  endif
#endif

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

static const int ALIGNMENT = 4;

static const char RCSid[] = "$Id: Buffer.cc,v 1.22 1996/04/15 10:59:15 nmcl Exp $";

static const char VERSION[4] = { '#', 'B', 'E', 0x1 };

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
	       : shortSize(sizeof(unsigned short)),
		 intSize(sizeof(unsigned int)),
		 longSize(sizeof(unsigned long)),
#ifdef HAVE_LONG_LONG
		 longlongSize(sizeof(unsigned long long)),
#else
		 longlongSize(0),
#endif
		 buffState(BUFFER_GOOD),
		 bufferEnd(0),
		 bufferStart(0),
		 rereadOffset(0),
		 chunkSize(buffSize),
		 packPoint(0),
		 unpackPoint(0),
		 shouldDelete(TRUE),
		 readOnly(FALSE)
{
#ifdef DEBUG
    debug_stream.lock();
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << chunkSize << ")" << endl;
    debug_stream.unlock();    
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
               : shortSize(sizeof(unsigned short)),
		 intSize(sizeof(unsigned int)),
		 longSize(sizeof(unsigned long)),
#ifdef HAVE_LONG_LONG
		 longlongSize(sizeof(unsigned long long)),
#else
		 longlongSize(0),
#endif
		 buffState(BUFFER_GOOD),
		 bufferEnd(0),
		 bufferStart(0),
		 rereadOffset(0),
		 chunkSize(DEFAULT_CHUNK_SIZE),
		 packPoint(0),
		 unpackPoint(0),
		 shouldDelete(shouldDel),
		 readOnly(FALSE)
{
#ifdef DEBUG
    debug_stream.lock();   
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << buffSize << ", " << (void *) buffer;
    debug_stream << ", " << full << ", " << shouldDel << ")" << endl;
    debug_stream.unlock();    
#endif

    setBuffer(buffSize, buffer, full, shouldDel);
}

/*
 * Initialise one buffer from another
 */

Buffer::Buffer ( const Buffer& copyFrom )                 
               : shortSize(sizeof(unsigned short)),
		 intSize(sizeof(unsigned int)),
		 longSize(sizeof(unsigned long)),
#ifdef HAVE_LONG_LONG
		 longlongSize(sizeof(unsigned long long)),
#else
		 longlongSize(0),
#endif
		 buffState(BUFFER_GOOD),
		 bufferEnd(0),
		 bufferStart(0),
		 rereadOffset(0),
		 chunkSize(0),
		 packPoint(0),
		 unpackPoint(0),
		 shouldDelete(TRUE),
		 readOnly(FALSE)
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */
    debug_stream.lock();
    debug_stream << CONSTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::Buffer (" << (void *)&copyFrom << ")" << endl;
    debug_stream.unlock();    
#endif

    copybuff(copyFrom);
}
     
Buffer::~Buffer ()
{
#ifdef DEBUG
    debug_stream.lock();    
    debug_stream << DESTRUCTORS << FAC_BUFFER_MAN << VIS_PUBLIC;
    debug_stream << "Buffer::~Buffer()" << endl;
    debug_stream.unlock();   
#endif

    if (shouldDelete && (bufferStart != 0))
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] bufferStart;
#else
	delete bufferStart;
#endif
	bufferStart = 0;
	rereadOffset = 0;
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

Boolean Buffer::shouldPack ( const void *ptr, Boolean supressMapCheck,
			     Uint32 hashval )
{
    Boolean doPack = FALSE;
    Int32 index = 0;

    if (packPointers.addPtr(ptr, index, supressMapCheck, hashval))
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

Boolean Buffer::saveUnpackedPtr ( const void *ptr, Int32& index )
{
    return unpackPointers.addPtr(ptr, index);
}

Boolean Buffer::unpackPtr ( const void*& ptr, Int32& index )
{
    Boolean unpackMe = FALSE;

    unpack(index);
    if (index >= 0)
    {
	ptr = unpackPointers[index];
    }
    else
    {
	unpackMe = TRUE;
    }
    
    return unpackMe;
}

Boolean Buffer::pack ( unsigned long l )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

    if (sizeof(unsigned long) <= sizeof(Uint32))
    {
	Uint32 u32 = l;
	return pack32bits(u32);
    }
    else
	if (sizeof(unsigned long) <= sizeof(Uint64))
	{
	    /*
	     * To get here we MUST have 64 bit longs so LONG_IS_64BITS
	     * should be defined and Uint64 will be a long. However, we
	     * have to keep all compilers happy so for machines with
	     * 32 bit longs we supress this code
	     */
#ifdef LONG_IS_64BITS
	    Uint64 u64 = l;
	    return pack64bits(u64);
#endif
	}
    
    buffState = BUFFER_BAD;
    return (FALSE);
}

#ifdef HAVE_LONG_LONG
Boolean Buffer::pack ( unsigned long long ll )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

    if (sizeof(unsigned long long) <= sizeof(Uint64))
    {
	Uint64 u64 = ll;
	return pack64bits(u64);
    }
    
    buffState = BUFFER_BAD;
    return (FALSE);
}
#endif

Boolean Buffer::pack ( float d )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
		return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX) || defined(WIN32)
    /*
     * Beware some compilers pass floats as doubles. So using &d
     * below would fail. Use temp var instead
     */
    
    float f = d;
    const Uint32 *uint32_p = (const Uint32 *) &f;

    return (pack32bits(*uint32_p));
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

Boolean Buffer::pack ( double d )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX) || defined(WIN32)
    const Uint64 *uint64_p = (const Uint64 *) &d;

    return (pack64bits(*uint64_p));
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

/* a null terminated string */

Boolean Buffer::packMappedString ( const char *string )
{
    size_t strSize = 0;
    
    if (string)
	strSize = ::strlen(string) + 1;
    
    return packBlock(string, strSize, FALSE);
}

Boolean Buffer::packNonMappedString ( const char *string )
{
    size_t strSize = 0;
    
    if (string)
	strSize = ::strlen(string) + 1;
    
    return packBlock(string, strSize, TRUE);
}

Boolean Buffer::packMappedBlock ( const void *memory, const size_t amount )
{  
    return packBlock(memory, amount, FALSE);
}

Boolean Buffer::packNonMappedBlock ( const void *memory, const size_t amount )
{  
    return packBlock(memory, amount, TRUE);
}

#ifdef HAVE_BOOLEAN_TYPE
Boolean Buffer::unpack ( bool& b )
{
    Uint32 l;
    Boolean res = unpack(l);

    if (res)
	b = (bool)l;

    return res;
}
#endif

Boolean Buffer::unpack ( unsigned char& c )
{
    Uint32 l;
    Boolean res = unpack(l);

    if (res)
	c = (unsigned char)l;

    return res;
}

Boolean Buffer::unpack ( unsigned short& s )
{
    Uint32 l;
    Boolean res = unpack(l);

    if (res)
	s = (unsigned short)l;

    return res;
}

Boolean Buffer::unpack ( unsigned int& i )
{
    Uint32 l;
    Boolean res = unpack32bits(l);

    if (res)
	i = (unsigned int)l;

    return res;

}

/*
 * Unpack integral quantity - assume unpackPoint is aligned (ala pack)
 * and let realign maintain that
 * Assume longs are at least 32 bits
 */

Boolean Buffer::unpack ( long& l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (longSize == sizeof(Uint32))	/* buffers longs are 32 bits */
    {
	Uint32 temp;
	Boolean res = unpack32bits(temp);
	
	l = temp;
	return res;
    }
    else
	if (longSize == sizeof(Uint64))
	{
	    /*
	     * Buffer longs are 64 bits. Whether I can unpack them
	     * depends on whether I have 64 bit longs or if the
	     * value will fit in 32 bits
	     */
	    
	    Uint64 temp;
	    
	    Boolean res = unpack64bits(temp);

	    if (res)
	    {
#ifdef LONG_IS_64BITS 
		l = temp;		/* true 64 bit result */
#else
#ifdef HAVE_LONG_LONG

		/* Check if value will fit */
		
		if ((temp >= LONG_MIN) && (temp <= LONG_MAX))
		    l = temp;
#else
		/*
		 * We do not have true compiler supported 64 bit ints
		 * use the fakes and make sure the (signed) number will fit
		 */

		if (((temp.hi == 0) && ((temp.lo & LONG_MIN) == 0)) ||
		    ((temp.hi == ~0) && ((temp.lo & LONG_MIN) != 0)))
		{
		    l = temp.lo;
		}
		else
		    res = FALSE;
#endif
#endif
	    }
	    
	    return res;
	}
    
    /* buffer longs are too big for me */

    buffState = BUFFER_BAD;
    return (FALSE);
}

Boolean Buffer::unpack ( unsigned long& l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (longSize == sizeof(Uint32))	/* buffers longs are 32 bits */
    {
	Uint32 temp;
	Boolean res = unpack32bits(temp);
	
	l = temp;
	return res;
    }
    else
	if (longSize == sizeof(Uint64))
	{
	    /*
	     * Buffer longs are 64 bits. Whether I can unpack them
	     * depends on whether I have 64 bit longs or if the
	     * value will fit in 32 bits
	     */
	    
	    Uint64 temp;
	    
	    Boolean res = unpack64bits(temp);

	    if (res)
	    {
#ifdef LONG_IS_64BITS 
		l = temp;		/* true 64 bit result */
#else
#ifdef HAVE_LONG_LONG

		/* Check if value will fit */
		
		if ((temp >= LONG_MIN) && (temp <= LONG_MAX))
		    l = temp;
#else
		/*
		 * We do not have true compiler supported 64 bit ints
		 * use the fakes and make sure the number will fit
		 */

		if (temp.hi == 0)
		{
		    l = temp.lo;
		}
		else
		    res = FALSE;
#endif
#endif
	    }
	    
	    return res;
	}
    
    /* buffer longs are too big for me */

    buffState = BUFFER_BAD;
    return (FALSE);
}

#ifdef HAVE_LONG_LONG
Boolean Buffer::unpack ( unsigned long long& ll )
{
    Uint64 l;
    Boolean res = unpack64bits(l);

    if (res)
	ll = (unsigned long long)l;

    return res;

}
#endif

Boolean Buffer::unpack ( float& f )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX) || defined(WIN32)
    Uint32 *uint32_p = (Uint32 *) &f;

    return (unpack32bits(*uint32_p));
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

Boolean Buffer::unpack ( double& d )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE) || defined(_AIX) || defined(WIN32)
    Uint64 *uint64_p = (Uint64 *) &d;

    return (unpack64bits(*uint64_p));
#else
    buffState = BUFFER_BAD;
    return (FALSE);
#endif
}

/* 
 * Unpack a null terminated string. 
 * Always Unpacks into a new area of memory
 */

Boolean Buffer::unpack ( char *& string )
{
    size_t slength = 0;
    void *p = 0;

    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if (unpackBlock(p, slength))
    {
	string = (char *)p;
	
	return TRUE;
    }

    return (FALSE);
}

/*
 * Operation to unpack a contiguous block of memory.
 * The long argument is set to the size unpacked.
 */

Boolean Buffer::unpack (void *& memory, size_t& amount )
{
    return unpackBlock(memory, amount);
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
    
    if (buff.pack(readOnly) &&
	buff.pack((Uint32)(packPoint - bufferStart)) &&
	buff.pack((Uint32)(unpackPoint - bufferStart)) &&
	(buff.pack(len)))
	if (len != 0)
	{
	    if (buff.packMappedBlock(bufferStart, len) &&
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
    void *tempBuffer = 0;
    Boolean res = FALSE;
    Uint32 packOffset = 0;
    Uint32 unpackOffset = 0;

    if (buff.unpack(readOnly) &&
	buff.unpack(packOffset) &&
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
    
    return packMappedBlock(from.unpackPoint, lenToPack);
    
}

Buffer& Buffer::operator= ( const Buffer& buff )
{
    copybuff(buff);
    return *this;
}

Buffer& Buffer::operator+= ( const Buffer& buff )
{
    if (buffState != BUFFER_GOOD ||
	readOnly ||
	buff.readState() != BUFFER_GOOD)
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
    /*
     * Require cast for Visual C++.
     */
    
    strm << "<" << (int) buffState;

    if (readOnly)
	strm << "(R)";
    
    strm << "><";
    strm << (void *) bufferStart << "->" << (void *) bufferEnd
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

/*
 * Change the underlying buffer. Since this contains flags they
 * need to be reset as well
 */

Boolean Buffer::setBuffer ( size_t buffSize, void *buffer, 
			    Boolean full,
			    Boolean shouldDel )
{
    if ((bufferStart != NULL) && shouldDelete)
	delete bufferStart;
    
    shouldDelete = shouldDel;
    bufferStart  = (char *)buffer;
    bufferEnd    = bufferStart+buffSize;
    packPoint    = (full ? bufferEnd : bufferStart);
    unpackPoint  = bufferStart;

    if (bufferStart == 0)
    {
	buffState = BUFFER_BAD;
	return FALSE;
    }
    
    /*
     * If the pack point is at Buffer start put in the sizes and flag
     * otherwise assume they are there already and extract them
     */

    if (packPoint == bufferStart)
    {    
	initBuffer();
    }
    else
    {
	/*
	 * Buffer contains data:
	 * Extract flag and if ok extract sizes too
	 */

	if ((::memcmp(bufferStart, VERSION, sizeof(VERSION)) == 0))
	{
	    /* Magic number matches - get sizes */
	    
	    Uint32 ui;

	    unpackPoint += sizeof(VERSION);
	    
	    unpack(ui);
	    
	    rereadOffset = unpackPoint - bufferStart;
	    
	    longlongSize = ui & 0xff;
	    longSize = (ui &0xff00) >> 8;
	    intSize = (ui &0xff0000) >> 16;
	    shortSize = (ui & 0xff000000) >> 24;

	    /* Check sizes are compatable */
	    
	    if ((shortSize != sizeof(unsigned short)) ||
		 (intSize != sizeof(unsigned int)) ||
		 (longSize != sizeof(unsigned long))
#ifdef HAVE_LONG_LONG
		 || (longlongSize != sizeof(unsigned long long))
#endif
		)
		readOnly = TRUE;
	}
	else
	{
	    /* Probably an old format buffer - mark as read-only just in case */
	    
	    readOnly = TRUE;

	    /* Old format buffers only had longs of 32 bits */

	    longSize = 4;
	    
	    unpackPoint = bufferStart;
	    rereadOffset = 0;
	    buffState = BUFFER_GOOD;    
	}

    }

    return (buffState == BUFFER_GOOD ? TRUE : FALSE);
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
    
    shortSize = buff.shortSize;
    intSize = buff.intSize;
    longSize = buff.longSize;
    longlongSize = buff.longlongSize;
    
    chunkSize = buff.chunkSize;

    packPointers = buff.packPointers;
    unpackPointers = buff.unpackPointers;
    readOnly = buff.readOnly;
    
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
 * Init a NEW Buffer. Places flag and sizes in first parts of buffer
 */

Boolean Buffer::initBuffer ()
{
    shortSize = sizeof(unsigned short);
    intSize = sizeof(unsigned int);
    longSize = sizeof(unsigned long);
#ifdef HAVE_LONG_LONG
    longlongSize = sizeof(unsigned long long);
#else
    longlongSize = 0;
#endif
    
    if (bufferStart != 0)
    {
	Uint32 ui = shortSize;
    
	/*
	 * The first entries are always the magic flag and the sizes of the basic types.
	 * Do not use pack individually as it would use 32 bits for each!
	 */
	
	ui = (ui << 8) | intSize;
	ui = (ui << 8) | longSize;
	ui = (ui << 8) | longlongSize;

	if (sizeup(sizeof(VERSION) + sizeof(Uint32)))
	{
	    ::memcpy(bufferStart, VERSION, sizeof(VERSION));
	    packPoint += sizeof(VERSION);
	    realign(packPoint);
	    pack(ui);

	    /* Force unpack point on so we do not mistake sizes as user data */

	    unpackPoint = packPoint;
	    rereadOffset = packPoint - bufferStart;
	}
	else
	    buffState = BUFFER_BAD;
    }
    
    return (buffState == BUFFER_BAD ? FALSE : TRUE);
}

Boolean Buffer::pack32bits ( Uint32 ui )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

    /* Ensure room exists */

    if (sizeup(sizeof(Uint32)) == FALSE)
    {
	buffState = BUFFER_BAD;
	return (FALSE);
    }

    /* and then pack */

    *((Uint32 *) packPoint) = htonl(ui);
    packPoint += sizeof(Uint32);
    
    return (realign(packPoint));
}

Boolean Buffer::pack64bits ( Uint64 ui )
{
    if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

    /* Ensure room exists */

    if (sizeup(sizeof(Uint64)) == FALSE)
    {
	buffState = BUFFER_BAD;
	return (FALSE);
    }

    /* and then pack */
#if defined(HAVE_LONG_LONG) || defined(LONG_IS_64BITS)
    return (pack32bits(ui >> 32) && pack32bits(ui & 0xffffffff));
#else
    return (pack32bits(ui.hi) && pack32bits(ui.lo));
#endif
}

Boolean Buffer::packBlock ( const void *memory, size_t amount,
			    Boolean supressMapCheck )
{
    Uint32 hashval = 0;
    
     if ((buffState != BUFFER_GOOD) || readOnly)
	return (FALSE);

    if (memory && (amount > 0))
	hashval = checkSum(memory, amount);
    
    if (shouldPack(memory,supressMapCheck, hashval))
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

/*
 * Unpack integral quantities - assume unpackPoint is aligned (ala pack)
 * and let realign maintain that
 */

Boolean Buffer::unpack32bits ( Uint32& l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if ((unpackPoint + sizeof(Uint32)) <= packPoint)
    {
	l = ntohl(*((Uint32 *) unpackPoint));
	unpackPoint += sizeof(Uint32);
	return (realign(unpackPoint));
    }
    buffState = BUFFER_BAD;
    return FALSE;
}

Boolean Buffer::unpack64bits ( Uint64& l )
{
    if (buffState != BUFFER_GOOD)
	return (FALSE);

    if ((unpackPoint + sizeof(Uint64)) <= packPoint)
    {
#if defined (LONG_IS_64BITS) || defined(HAVE_LONG_LONG)
	Uint32 temp;
	Boolean res = unpack32bits(temp);

	if (res)
	{
	    l = temp;
	    res = unpack32bits(temp);
	    if (res)
		l = (l << 32) | temp;
	}
	return res;
#else
	return (unpack32bits(l.hi) && unpack32bits(l.lo));
#endif
    }
    buffState = BUFFER_BAD;
    return FALSE;
}

/*
 * Operation to unpack a contiguous block of memory.
 * The long argument is set to the size unpacked.
 */

Boolean Buffer::unpackBlock (void *& memory, size_t& amount )
{
    Int32 index  = 0;
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
#ifndef GCC_ARRAY_DELETE_BUG
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
	    else
	    {
		/* Preload initial buffer contents */
		initBuffer();
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
