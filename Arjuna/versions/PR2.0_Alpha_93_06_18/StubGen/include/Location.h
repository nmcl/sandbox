/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Location.h,v 1.5 1993/06/17 15:08:10 ngdp Exp $
 */

#ifndef LOCATION_H_
#define LOCATION_H_

/*
 *
 * Stub generator file location tracking. Used in error reporting
 * and parsing to note locations in files of declarations
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

class ostream;

class Location
{
public:
    Location ();
    Location (const Location&);
    Location (const String&, long);

    /* public operations */

    Location& operator= (const Location&);

    void incrLine ();
    void setFileName (const String&);
    void setLineno (long);

    String getFileName () const;
    long getLineno () const;

    void printLoc (ostream&) const;

private:
    /* private state */

    String file;
    long lineno;

};

#include "Location.n"

#endif
