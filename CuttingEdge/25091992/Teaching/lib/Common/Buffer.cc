/*
 * Copyright 1990, 1991, 1992
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * $Id: Buffer.cc,v 1.11 1992/02/28 15:36:14 ngdp Exp $
 */

/*
 *
 * Class that implements the marshalling operations for the primitive C types
 * supported by C++ (with certain obvious exceptions).
 * The internal char * buffer is managed internally and will be freed
 * when the Buffer instance is destroyed REGARDLESS of where it was created.
 *
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream.h>

#include <Common/Buffer.h>

static const char RCSid[] = "$Id: Buffer.cc,v 1.11 1992/02/28 15:36:14 ngdp Exp $";

/* 
 * Public constructors and destructors
 */

/*
 * The following constructor is provided for creating an empty Buffer.
 * The argument may be used to define the amount of memory allocated
 * each time extra memory is required - it defaults to the value
 * in Buffer.h 
 */

Buffer::Buffer ( size_t default_chunk_size )
                 : buff_state(BUFFER_GOOD),
		   buffer_start(0),
		   buffer_end(0),
		   pack_point(0),
		   unpack_point(0),
		   chunk_size(default_chunk_size),
		   should_delete(TRUE)

{
}

/*
 * The following constructor is provided for creating a Buffer to manage
 * an existing block of memory, or for creating a Buffer when the size of
 * the Buffer is known beforehand. If the boolean argument is TRUE then
 * the char* buffer is assumed to already contain data. If FALSE then
 * the size_t argument defines the size of the EMPTY char* buffer.
 */

Buffer::Buffer ( size_t s, char *b, Boolean full_buffer, Boolean should_del ) 
               : buff_state(BUFFER_GOOD),
		 buffer_start(b),
		 buffer_end(b+s),
		 pack_point(full_buffer ? b+s : b),
		 unpack_point(b),
		 chunk_size(DEFAULT_CHUNK_SIZE),
		 should_delete(should_del)
                                                 
{
}

/*
 * Initialise one buffer from another
 */

Buffer::Buffer ( const Buffer& copyfrom )                 
               : buff_state(BUFFER_GOOD),
		 buffer_start(0),
		 buffer_end(0),
		 pack_point(0),
		 unpack_point(0),
		 chunk_size(0),
		 should_delete(TRUE)
{
    copybuff(copyfrom);
}
     
Buffer::~Buffer ()
{
    if (should_delete && (buffer_start != 0))
	delete buffer_start;
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
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    /* Ensure room exists */

    if (sizeup(sizeof(unsigned long)) == FALSE)
    {
	buff_state = BUFFER_BAD;
	return (FALSE);
    }

    /* and then pack */

    *((unsigned long *) pack_point) = htonl(l);
    pack_point += sizeof(unsigned long);
    
    return (realign(pack_point));
}

Boolean Buffer::pack ( const double d )
{
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE)
    const long *long_p = (const long *) &d;

    return ((pack(*long_p) && pack(*(++long_p))) ? TRUE : FALSE);
#else
    buff_state = BUFFER_BAD;
    return (FALSE);
#endif
}

/* a null terminated string */

Boolean Buffer::pack ( const char *string )
{
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    if (string)
    {
	size_t slength = strlen(string);

	if (pack(slength) && sizeup(slength))
	{
	    if (slength > 0)
	    {
		bcopy(string, pack_point, slength);
		pack_point += slength;
	    }

	}
	else
	    return FALSE;		/* buffer already bad */	    
    }
    else
	pack(-1);

    return (realign(pack_point));
}

/* operation to pack a contiguous area of memory */

Boolean Buffer::pack ( const char *memory, const size_t amount )
{
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    if (memory)
    {
	if (pack(amount) && sizeup(amount))
	{
	    if (amount > 0)
	    {
		bcopy(memory, pack_point, (int) amount);
		pack_point += amount;
	    }
	}
	else
	    return FALSE;		/* buffer already bad */	    
    }
    else
	pack (-1);

    return (realign(pack_point));
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
 * Unpack integral quantity - assume unpack_point is aligned (ala pack)
 * and let realign maintain that
 */

Boolean Buffer::unpack ( unsigned long& l )
{
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    if ((unpack_point + sizeof(unsigned long)) <= pack_point)
    {
	l = ntohl(*((long *) unpack_point));
	unpack_point += sizeof(unsigned long);
	return (realign(unpack_point));
    }

    buff_state = BUFFER_BAD;
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
    if (buff_state != BUFFER_GOOD)
	return (FALSE);

#if defined(mc68000) || defined(sparc) || defined(_IEEE)
    long *long_p = (long *) &d;

    return ((unpack(*long_p) && unpack(*(++long_p))) ? TRUE : FALSE);
#else
    buff_state = BUFFER_BAD;
    return (FALSE);
#endif
}

/* 
 * Unpack a null terminated string. If reuse if FALSE - the default
 * the old memory is deleted and a new area created otherwise we
 * attempt to reuse the memory
 */

Boolean Buffer::unpack ( char* &string, Boolean reuse )
{
    size_t slength;

    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    if (unpack(slength))
    {
	/* If there is memory already and its not to be used again */
	
	if (slength == -1)
	{
	    string = 0;
	    return TRUE;
	}

	if (reuse == FALSE)
	{
	    if (string)
		delete string;

	    string = new char[slength + 1];

	}

	if (string)
	{
	    bcopy(unpack_point, string, slength);
	    (string)[slength] = '\0';
	    unpack_point += slength;
	    return (realign(unpack_point));
	}
    }

    buff_state = BUFFER_BAD;
    return (FALSE);
}

/*
 * Operation to unpack a contiguous block of memory.
 * The long argument is set to the size unpacked.
 */

Boolean Buffer::unpack (char *& memory, size_t& amount )
{

    size_t mem_size;

    if (buff_state != BUFFER_GOOD)
	return (FALSE);

    if (unpack(mem_size))
    {
	if (mem_size == -1)
	{
	    memory = 0;
	    amount = 0;
	    return TRUE;
	}

	if (memory == 0)
	    memory = new char[mem_size];
	else
	{
	    /* alocated memory large enough? */
	    if (mem_size > amount)
	    {
		amount = mem_size;

		/* always (un)pack ints etc as unsigned longs */

		unpack_point -= sizeof(unsigned long);
		return (FALSE);
	    }
	}

	if (memory)
	{
	    bcopy(unpack_point, memory, (int) amount);
	    unpack_point += amount;
	    return (realign(unpack_point));
	}
    }

    buff_state = BUFFER_BAD;
    return (FALSE);
}

/*
 * Pack this buffer into another. Note that we don't call this simply
 * pack because it is the INVERSE of the normal pack. That is 'this' is
 * packed into the argument buffer, whereas normally (for standadr types)
 * the argument is packed into 'this'
 */

Boolean Buffer::pack_into ( Buffer& buff ) const
{
    Boolean res;
    size_t len = length();
    
    if ((res = buff.pack(len)) &&
	(len != 0))
	res = buff.pack(buffer_start, len);
    
    return res;
}

/*
 * Unpack this from the arg buffer. Obeys same rules as pack_into above
 */

Boolean Buffer::unpack_from ( Buffer& buff )
{
    size_t buff_size;
    char *temp_buffer = 0;
    Boolean res;

    if ((res = buff.unpack(buff_size)) && (buff_size != 0))
    {
	if ((res = buff.unpack(temp_buffer, buff_size)) &&
	    (temp_buffer != 0))
	    res = set_buffer(buff_size, temp_buffer);
    }

    return res;
}

Buffer& Buffer::operator= ( const Buffer& buff )
{
    copybuff(buff);
    return *this;
}

Buffer& Buffer::operator+= ( const Buffer& buff )
{
    if (buff_state != BUFFER_GOOD || buff.readstate() != BUFFER_GOOD)
	return (*this);

    if (buff.length() > 0)
    {
	if (sizeup(buff.length()))
	{
	    bcopy(buff.buffer_start, pack_point, (int) buff.length());
	    pack_point += buff.length();
	    realign(pack_point);
	}
	else
	    buff_state = BUFFER_BAD;
    }
    return *this;
}

/*
 * Public virtual functions
 */

ostream& Buffer::print ( ostream& strm ) const
{
    strm << "<"
         << (void *) buffer_start << "->" << (void *) buffer_end
	 << "><"
	 << (void *) pack_point << "," << (void *) unpack_point << ">(";
    
    if ((buffer_start != 0) && (pack_point != 0))
    {
	for (unsigned int i = 0; (buffer_start + i) <= pack_point; i++)
	{
	    strm << (int) (buffer_start[i] & 0xff);
	    if (isprint(buffer_start[i]))
		strm << " [" << buffer_start[i] << "] ";
	    else
		strm << " {?} " ;
	}
	strm << dec;
    }
    strm << dec << ")";

    return strm;
}

Boolean Buffer::set_buffer ( size_t s, char *b, 
			     Boolean full_buffer,
			     Boolean should_del )
{
    if (buffer_start == 0)
    {
	should_delete = should_del;
	buffer_start  = b;
	buffer_end    = b+s;
	pack_point    = (full_buffer ? b+s : b);
	unpack_point  = b;

	return TRUE;
    }

    return FALSE;
}

/*
 * Private non-virtual functions
 */

/*
 * Make a copy of the argument buffer in the current one
 */

void Buffer::copybuff ( const Buffer& buff )
{
    if (buff_state != BUFFER_GOOD || buff.readstate() != BUFFER_GOOD)
	return;

    /*
     * copy over parameters first
     */

    chunk_size = buff.chunk_size;
   
    /*
     * Only copy buffer if actually necessary
     */

    if (buff.length() > 0)
    {
	if (sizeup(buff.length()))
	{
	    bcopy(buff.buffer_start, buffer_start, (unsigned int) buff.length());
	    pack_point = buffer_start + buff.length();

	    if (buff.unpack_point > buff.buffer_start)
		unpack_point = buffer_start +
		    (buff.unpack_point - buff.buffer_start);
	    else
		unpack_point = buffer_start;
	}
	else
	    buff_state = BUFFER_BAD;
    }
    else
	buffer_start = buffer_end = pack_point = unpack_point = (char *) 0;
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
	    buff_state = BUFFER_BAD;
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
    if (buff_state != BUFFER_GOOD)
	return (FALSE);
    
    if ((pack_point + extra) > buffer_end)
    {
	size_t new_size;
	size_t old_size = length();
	char *old_start = buffer_start;
	
	/* set new size */
	new_size = old_size + extra;
	
	/* round up to nearest multiple of chunk_size */
	if (new_size % chunk_size)
	    new_size += chunk_size - (new_size % chunk_size);
	
	if (buffer_start == 0)
	    buffer_start = pack_point = unpack_point = new char[new_size];
	else
	{
	    /* realloc */
	    
	    buffer_start = new char[new_size];
	    if (buffer_start)
	    {
		bcopy(old_start, buffer_start, old_size);
		if (should_delete)
		    delete old_start;
	    }
	}
	
	should_delete = TRUE;
	
	if (buffer_start == 0)
	{
	    buffer_start = old_start;
	    
	    cerr << "Buffer::sizeup malloc/realloc failure\n";
	    buff_state = BUFFER_BAD;
	    return (FALSE);
	}
	else
	{
	    buffer_end = buffer_start + new_size;
	    
	    if (old_start)
	    {
		/* buffer start MUST have changed by this point. So.. */
		pack_point = buffer_start + (pack_point - old_start);
		unpack_point = buffer_start + (unpack_point - old_start);
	    }
	    bzero(pack_point, buffer_end - pack_point);
	}
    }

     return (TRUE);
 }

char *Buffer::buffer () const
{
    return buffer_start;
}

size_t Buffer::length () const
{
    return (pack_point - buffer_start);
}

Buffer_State Buffer::readstate () const
{
    return buff_state;
}
void Buffer::rewind ()
{
    pack_point = unpack_point = buffer_start;
}

void Buffer::set_length ( size_t new_length )
{
    pack_point = buffer_start + new_length;
}

Boolean Buffer::pack ( const char c )
{
    return (pack((const unsigned long) c));
}

Boolean Buffer::pack ( const unsigned char c )
{
    return (pack((const unsigned long) c));
}

Boolean Buffer::pack ( const short s )
{
    return (pack((const unsigned long) s));
}

Boolean Buffer::pack ( const unsigned short s )
{
    return (pack((const unsigned long) s));
}

Boolean Buffer::pack ( const int i )
{
    return (pack((const unsigned long) i));
}

Boolean Buffer::pack ( const unsigned int i )
{
    return (pack((const unsigned long) i));
}

Boolean Buffer::pack ( const long l )
{
    return (pack((const unsigned long) l));
}

Boolean Buffer::pack ( const float f )
{
    double d = f;

    return (pack(d));
}

Boolean Buffer::unpack ( char& c )
{
    return (unpack((unsigned char&) c));
}

Boolean Buffer::unpack ( short& s )
{
    return (unpack((unsigned short&) s));
}

Boolean Buffer::unpack ( int& i )
{
    return (unpack((unsigned int&) i));
}

Boolean Buffer::unpack ( long& l )
{
    return (unpack((unsigned long&) l));
}

ostream& operator<<  ( ostream& strm, const Buffer& b )
{
    return b.print (strm);
}

