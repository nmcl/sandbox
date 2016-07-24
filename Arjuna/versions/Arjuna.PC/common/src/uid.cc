/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: Uid.cc,v 1.3 1993/11/24 12:34:49 ngdp Exp $
 */

/*
 * Unique Identifier implementation. This version uses the internet
 * number of the creating host concatenated with timestamp information.
 */



#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifdef MSDOS
#  ifndef TIME_H_
#    include <System/time.h>
#  endif
#else
#  ifndef SYS_TIME_H_
#    include <System/sys/time.h>
#  endif
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
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

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

static const char RCSid[] = "$Id: Uid.cc,v 1.3 1993/11/24 12:34:49 ngdp Exp $";

/*
 * The NIL_UID
 */


#ifdef OS2
const Uid NIL_UID("0_0_0_0");
#else
const Uid NIL_UID("0.0");
#endif


/*
 * This will *not* generate unique numbers in a multi-threaded environment.
 */

#ifdef MSDOS
static unsigned long bogusTime = 0;
#endif

static unsigned long uidsCreated = 0;

/*
 * Public constructors and destructor
 */

Uid::Uid ()
{
    hostAddr = 0;
	process = 0;

#ifndef MSDOS
    sec = ::time(0);    
    other = uidsCreated++;
#else
    sec = ::time(0);
    other = uidsCreated++;
    if (bogusTime > sec)  // use warped time!
	sec = bogusTime;

    if (other > 255)
    {
	bogusTime = sec = sec++;  // move time on!
	other = uidsCreated = 0;
    }
#endif
}

Uid::Uid (const Uid& copyFrom )
{
    *this = copyFrom;
}

/*
 * Create Uid from string representation. Boolean arg says whether
 * to give up if an error is detected or to simply replace with NIL_UID
 */

Uid::Uid ( const char *uidString, Boolean errsOk )
{
    int slen = (uidString != 0 ? strlen(uidString): 0);
    char *cp = new char[slen+1];
	char *breakp = 0;
#ifndef MSDOS
	Boolean newFormat = TRUE;
#endif
    char dummy;

    /* Copy incoming string and split into two parts */

    if (slen > 0)
    {        
	::strcpy(cp, uidString);
#ifdef OS2
	breakp = ::strchr(cp, '_');
#else
	breakp = ::strchr(cp, '.');
#endif
        if (breakp)
            *breakp = 0;

#ifdef OS2
	*breakp = '_';
	breakp = cp;
#else
	*breakp = '.';
	breakp = cp;
#endif

	/* Extract fields with istream ops */
	
	istrstream uid_source(breakp);

#ifndef MSDOS	
	if (newFormat)
	{
	    hex(uid_source);
	    uid_source >> hostAddr >> dummy;
	}
#endif

#ifndef MSDOS	
	uid_source >> process >> dummy;
#endif
	uid_source >> sec >> dummy;
	uid_source >> other;

#ifndef MSDOS
	if ((hostAddr == -1) || uid_source.bad())
	{
	    if (errsOk)
		*this = NIL_UID;
	    else
	    {
		error_stream << FATAL << "Uid::Uid string constructor incorrect\n";
		ArjunaFatal();
	    }
	}
#endif
    }
    else
    {
	if (errsOk)
		*this = NIL_UID;
	    else
	    {
		error_stream << FATAL << 
		    "Uid::Uid(char *) constructor called with NULL pointer\n";
		ArjunaFatal();
	    }
    }

#ifndef __GNUG__
    delete [] cp;
#else
    delete cp;
#endif
}

Uid::~Uid ()
{
}

/*
 * Public non-virtual functions and operators
 */

/*
 * Function to return a value that can be used as an index in a hash table
 * which has been ordered using Uids. 
 */ 

unsigned long Uid::hash ( ) const
{
    char buffer[256];			/* conversion buffer */
    strstream tos(buffer, 256, ios::out);
    register const char *cp;
    register unsigned long g, h = 0;

    tos << *this << ends;

    cp = buffer;

    while (cp && (*cp != '\0'))
    {
        h = (h << 4) + (*cp);
        if (g = h & 0xf0000000)
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
        cp++;
    }

    return h;
}

/*
 * Routines to pack and unpack Uids into Buffers (for RPC, storage etc)
 */

Boolean Uid::pack ( Buffer& packInto ) const
{
    return ((packInto.pack(hostAddr) && packInto.pack(process) &&
	     packInto.pack(sec) && packInto.pack(other)));
}

ostream& Uid::print ( ostream& strm ) const
{
    hex(strm);

#ifdef OS2
    strm << hostAddr << "_" << process << "_" << sec << "_" << other;
#else
    char uidBuff1[12], uidBuff2[12];
    char name1[8], name2[4];

    ::memset(uidBuff1, '\0', 12);
    ::memset(uidBuff2, '\0', 12);
    ::memset(name1, '\0', 8);
    ::memset(name2, '\0', 4);

	ostrstream os1(uidBuff1, 12, ios::out);
    ostrstream os2(uidBuff2, 12, ios::out);
    hex(os1); hex(os2);

    os1 << sec;
	os2 << other;

    // remove 0x from strings if any!

    if ((uidBuff1[0] == '0') && (uidBuff1[1] == 'x'))
		::strcpy(name1, uidBuff1+2);
	else
		::strcpy(name1, uidBuff1);

	if ((uidBuff2[0] == '0') && (uidBuff2[1] == 'x'))
		::strcpy(name2, uidBuff2+2);
	else
		::strcpy(name2, uidBuff2);

	strm << name1 << "." << name2;
#endif    
    dec(strm);
	return strm;
}

Boolean Uid::unpack ( Buffer& unpackFrom )
{
    return ((unpackFrom.unpack(hostAddr) && unpackFrom.unpack(process) &&
	     unpackFrom.unpack(sec) &&unpackFrom.unpack(other)));
}

/*
 * Uid comparisons
 */

int Uid::operator== ( const Uid& u ) const
{
    return ((other == u.other) &&
	    (sec == u.sec) &&
	    (process == u.process) &&
	    (hostAddr == u.hostAddr));
}

int Uid::operator!= ( const Uid& u ) const
{
    return ((other != u.other) ||
	    (sec != u.sec) ||
	    (process != u.process) ||
	    (hostAddr != u.hostAddr));
}

int Uid::operator< ( const Uid& u ) const
{
    if (hostAddr < u.hostAddr)
	return TRUE;
    else if (hostAddr == u.hostAddr)
    {
	if (process < u.process)
	    return TRUE;
	else if (process == u.process)
	{
	    if (sec < u.sec)
		return TRUE;
	    else if ((sec == u.sec) && (other < u.other))
		return TRUE;
	}
    }
    return FALSE;
}

int Uid::operator> ( const Uid& u ) const
{
    if (hostAddr > u.hostAddr)
	return TRUE;
    else if (hostAddr == u.hostAddr)
    {
	if (process > u.process)
	    return TRUE;
	else if (process == u.process)
	{
	    if (sec > u.sec)
		return TRUE;
	    else if ((sec == u.sec) && (other > u.other))
		return TRUE;
	}
    }
    return FALSE;
}

unsigned long Uid::hostInetAddr ()
{
	static unsigned long myAddr;	/* init to 0 by definition */

    return myAddr;
}

#ifdef NO_INLINES
#  define UID_CC_
#  include <Common/Uid.n>
#  undef UID_CC_
#endif
