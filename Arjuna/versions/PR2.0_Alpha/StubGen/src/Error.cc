/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.cc,v 1.6 1993/03/22 09:31:02 ngdp Exp $
 */

/*
 * Stub generator error handler
 *
 */

#include <strstream.h>
#include <stdlib.h>

#ifndef ERROR_H_
#  include "Error.h"
#endif

static const char RCSid[] = "$Id: Error.cc,v 1.6 1993/03/22 09:31:02 ngdp Exp $";

Error error_log;		       	/* ensure global error reporter */
					/* exists  */

Location error_location;	       	/* error location information */
ostrstream err_stream;
ostrstream& error_stream = err_stream;

Error::Error ()
             : errorcount(0),
	       warningcount(0),
	       warnings_wanted(FALSE)
{
}

void Error::post_error ( ostrstream& s, ErrorType e )
{
    if (e != WARNING  || warnings_wanted)
    {
	cerr << error_location << ": ";
    }

     switch (e)
     {
     case WARNING:
        if (warnings_wanted)
        {
            cerr << "Warning: ";
            cerr << s.rdbuf() << "\n";
        }
	else
	{
	    /* discard contents of 's' the hard way */
	    streambuf *sb = s.rdbuf();
//	    while (sb->snextc() != EOF);
	    sb->seekoff(0, ios::end, ios::in);
	    
	}
        warningcount++;
        break;
    case ERROR:
        cerr << "Error: ";
        cerr << s.rdbuf() << "\n";
        errorcount++;
        break;
    case FATAL:
        cerr << "Fatal error: ";
        cerr << s.rdbuf() << "\n";
    }
    cerr << flush;

    if (e == FATAL)
        abort();
}

void Error::summary ( ostream& s )
{
    if ((errorcount > 0) || (warningcount > 0))
    {
	s << errorcount << " error";
	if (errorcount != 1)
	    s << "s";
	s << ", " << warningcount << " warning";
	if (warningcount != 1)
	    s << "s";
	s << "\n";
    }
}

/*
 * Overload error, warning & fatal to take simple char * arguements
 */

void error ( char *s )
{
    ostrstream os;

    os << s;
    error_log.post_error(os, ERROR);
}

void warning ( char *s )
{
    ostrstream os;

    os << s;
    error_log.post_error(os, WARNING);
}

void fatal ( char *s )
{
    ostrstream os;

    os << s;
    error_log.post_error(os, FATAL);
}

#ifdef NO_INLINES
#  define ERROR_CC_
#  include "Error.n"
#  undef ERROR_CC_
#endif
