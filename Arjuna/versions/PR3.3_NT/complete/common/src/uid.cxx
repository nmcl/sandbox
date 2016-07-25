/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: Uid.cc,v 1.15 1996/04/25 13:51:41 ngdp Exp $
 */

/*
 * 
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

#ifndef WIN32

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#else

#  include <System/winsock.h>
#  include <System/time.h>

#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
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

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif


static const char RCSid[] = "$Id: Uid.cc,v 1.15 1996/04/25 13:51:41 ngdp Exp $";

/*
 * The NIL_UID
 */

static Uid *_arjuna_nil_uid = 0;

static Uint32 uidsCreated = 0;

/*
 * We do not use MAXHOSTNAMELEN here as it varies from machine
 * to machine and OS to OS and with binary compatability it might
 * be the case that what works in one place might fail on another
 * This value should be plenty big enough
 */

static const short MAXHOSTLEN = 512;

const Uid& Uid::_arjunaNilUid_ ()
{
    if (_arjuna_nil_uid == 0)
#ifndef WIN32
		_arjuna_nil_uid = new Uid("0:0:0:0");
#else
		_arjuna_nil_uid = new Uid("0_0_0_0");
#endif
    
    return *_arjuna_nil_uid;
}

/*
 * Public constructors and destructor
 */

Uid::Uid ()
	 : hostAddr(-1),
	   process(0),
	   sec(0),
	   other(0)
{
    hostAddr = hostInetAddr();		/* calculated only once */
    process = ::getpid();

    sec = ::time(0);
    
    other = uidsCreated++;

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
	 : hostAddr(-1),
	   process(0),
	   sec(0),
	   other(0)
{
    int slen = (uidString != 0 ? strlen(uidString): 0);
    char *cp = new char[slen+1];
    char *breakp = 0;
    Boolean newFormat = TRUE;
    Boolean uidOk = FALSE;
    char dummy = ' ';

    /* Copy incoming string and split into two parts */

    if (slen > 0)
    {        
		::strcpy(cp, uidString);
#ifndef WIN32
		breakp = ::strchr(cp, ':');
#else
		breakp = ::strchr(cp, '_');
#endif
		
		if (breakp)
		{
#ifdef SUPPORT_OLD_UIDS
				*breakp = 0;

			/* Pull out inet addr from first part */

			if ((strchr(cp, '.') != 0) && (hostAddr = inet_addr(cp)) != -1)
			{
				/* old format uid */
				newFormat = FALSE;
				breakp++;
			}
			else
			{
				/* new hex format */
#ifndef WIN32
				*breakp = ':';
#else
				*breakp = '_';
#endif
				breakp = cp;
			}
#else
			breakp = cp;
			
#endif
			/* Extract fields with istream ops */
		
			istrstream uid_source(breakp);
		
			if (newFormat)
			{
				hex(uid_source);
				uid_source >> hostAddr >> dummy;
			}
		
			uid_source >> process >> dummy;
			uid_source >> sec >> dummy;
			uid_source >> other;

			if ((hostAddr != -1) && (!uid_source.bad()))
				uidOk = TRUE;
		}
    }
    
    if(!uidOk)
    {
		if (errsOk)
			*this = NIL_UID;
		else
		{
			error_stream << FATAL << "Uid::Uid string constructor incorrect\n";
			ArjunaFatal();
		}
    }

#ifndef GCC_ARRAY_DELETE_BUG
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

    tos << *this << ends;

    return checkSum(buffer, ::strlen(buffer));
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
#ifndef WIN32
    return strm << hex << hostAddr << ":" << process << ":" << sec << ":" 
		<< other << dec;
#else
	return strm << hex << hostAddr << "_" << process << "_" << sec << "_"
		<< other << dec;
#endif
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

Uint32 Uid::hostInetAddr ()
{
    static Uint32 myAddr = 0;
    static char hostname[MAXHOSTLEN];

    if (myAddr == 0)
    {
		struct hostent *h = 0;
		
		::memset(hostname, 0, MAXHOSTLEN);
		::gethostname(hostname, MAXHOSTLEN);
		h = ::gethostbyname(hostname);
		::memcpy(&myAddr, h->h_addr, h->h_length);
    }

    return myAddr;
}

#ifdef NO_INLINES
#  define UID_CC_
#  include <Common/Uid.n>
#  undef UID_CC_
#endif
