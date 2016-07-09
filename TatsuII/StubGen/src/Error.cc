/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $
 */

/*
 * Stub generator error handler
 *
 */

#include <strstream>
#include <stdlib.h>

#ifndef ERROR_H_
#  include "Error.h"
#endif

static const char RCSid[] = "$Id: Error.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $";

Error errorLog;		       	/* ensure global error reporter */
					/* exists  */

Location errorLocation;	       	/* error location information */
ostrstream errStream;
ostrstream& errorStream = errStream;

Error::Error ()
             : errorCount(0),
	       warningCount(0),
	       supressedCount(0),
	       warningsWanted(FALSE)
{
}

void Error::postError ( ostrstream& s, ErrorType e )
{
     switch (e)
     {
     case WARNING:
     case CAUTION:
        if ((warningsWanted)  && !warningsSupressed)
        {
	    cerr << errorLocation << ": ";
	    if (e == CAUTION)
		cerr << "Caution: ";
	    else
		cerr << "Warning: ";
            cerr << s.rdbuf() << "\n";
        }
	else
	{
	    /* discard contents of 's' */
	    streambuf *sb = s.rdbuf();
	    sb->pubseekoff(0, ios_base::end, ios_base::in);
	}
        warningCount++;
	if (warningsSupressed)
	    supressedCount++;
	
        break;
     case ERROR:
     case FATAL:
	cerr << errorLocation << ": ";
	if (e == ERROR)
	    cerr << "Error: ";
	else
	    cerr << "Fatal error: ";
        cerr << s.rdbuf() << "\n";
        errorCount++;
        break;
    }
    cerr << flush;

    if (e == FATAL)
        abort();
}

void Error::summary ( ostream& s )
{
    if ((errorCount > 0) || (warningCount > 0))
    {
	s << errorCount << " error";
	if (errorCount != 1)
	    s << "s";
	s << ", " << warningCount << " warning";
	if (warningCount != 1)
	    s << "s";

	if (supressedCount > 0)
	{
	    s << " (" << supressedCount << " supressed)";
	}
	
	s << "\n";
    }
}

/*
 * Overload error, warning & fatal to take simple char * arguements
 */

void error ( const char *s )
{
    ostrstream os;

    os << s;
    errorLog.postError(os, ERROR);
}

void caution ( const char *s )
{
    ostrstream os;

    os << s;
    errorLog.postError(os, CAUTION);
}

void warning ( const char *s )
{
    ostrstream os;

    os << s;
    errorLog.postError(os, WARNING);
}

void fatal ( const char *s )
{
    ostrstream os;

    os << s;
    errorLog.postError(os, FATAL);
}

#ifdef NO_INLINES
#  define ERROR_CC_
#  include "Error.n"
#  undef ERROR_CC_
#endif
