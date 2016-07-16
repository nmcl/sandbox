/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Location.h,v 1.1 1997/09/25 15:30:16 nmcl Exp $
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

#ifndef STRINGCLASS_H_
#  include "StringClass.h"
#endif

#include <ostream>

using namespace std;


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
