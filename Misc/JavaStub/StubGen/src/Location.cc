/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Location.cc,v 1.1 1993/11/03 14:48:03 nmcl Exp $
 */

/*
 *
 * Stub generator file location tracking. Used in error reporting
 * and parsing to note locations in files of declarations
 *
 */

#include <strstream.h>

#ifndef LOCATION_H_
#  include "Location.h"
#endif

static const char RCSid[] = "$Id: Location.cc,v 1.1 1993/11/03 14:48:03 nmcl Exp $";

Location::Location ()
                   : file("<stdin>"),
		     lineno(0)
{
}

Location::Location ( const  Location& copyfrom )
                   : file(copyfrom.file),
		     lineno(copyfrom.lineno)
{
}

Location::Location ( const String& fname, long where )
                   : file(fname),
		     lineno(where)
{
}

void Location::printLoc ( ostream& s ) const
{
    s << "\"" << file << "\", line " << lineno;
}

Location& Location::operator= ( const  Location& otherloc )
{
    file = otherloc.file;
    lineno = otherloc.lineno;
    return *this;
}

#ifdef NO_INLINES
#  define LOCATION_CC_
#  include "Location.n"
#  undef LOCATION_CC_
#endif
