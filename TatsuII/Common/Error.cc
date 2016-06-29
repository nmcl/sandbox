/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.cc,v 1.1 1997/06/09 19:51:47 nmcl Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef TIME_H_
#  include <System/time.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

static const char RCSid[] = "$Id: Error.cc,v 1.1 1997/06/09 19:51:47 nmcl Exp $";

Error::Error ( FacilityCode err, int code,
	       const char * const msg, ErrorSeverity level )
	     : errCode(code),
	       errorFacility(err),
	       errorLevel(level),
	       errorHead(0),
	       errorText(clone(msg)),
	       nextError(0)
{
    ostrstream temp;

    temp << level << ends;

    errorHead = temp.str();

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << ERROR_MESSAGES << FAC_GENERAL << VIS_PUBLIC;
    debug_stream << *this << endl;
    debug_stream.unlock();
    
#endif

    /* Always write error message to cerr regardless */

    cerr << *this << endl;
    
}

Error::Error ()
	     : errorFacility(FAC_ALL),
	       errorLevel(WARNING),
	       errorHead(0),
	       errorText(0),
	       nextError(0)
{
}

Error::~Error ( )
{
    if (errorHead)
#ifndef GCC_ARRAY_DELETE_BUG
      ::delete [] errorHead;
#else
      ::delete errorHead;
#endif

    if (errorText)
#ifndef GCC_ARRAY_DELETE_BUG
      ::delete [] errorText;
#else
      ::delete errorText;
#endif

}

ostream& Error::print ( ostream& strm ) const
{
    if (errorHead)
	strm << errorHead;

    if (errorText)
	strm << errorText;

    return strm;
}
	
Error *Error::getLink () const
{
    return nextError;
}

void Error::setLink ( Error *pointTo )
{
    nextError = pointTo;
}

		 
ostream& operator<< ( ostream& strm, enum ErrorSeverity es )
{
    time_t theTime = time(0);
    char *tmp = ctime(&theTime);

    /* remember that ctime fields are constant in width */

    tmp += 4;				/* Skip day */
    tmp[15] = 0;			/* Supress year and newline */
    
    strm << (es == FATAL ? "FATAL ERROR" : "WARNING");
    strm << "(pid:" << getpid() <<  " on " << tmp
	 << "): ";
    
    return strm;
}

void ArjunaFatal ()
{
    abort();
}

#ifdef NO_INLINES
#  define ERROR_CC_
#  include <Common/Error.n>
#  undef ERROR_CC_
#endif
