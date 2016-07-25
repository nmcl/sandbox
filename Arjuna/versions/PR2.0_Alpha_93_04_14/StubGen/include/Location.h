/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Location.h,v 1.4 1993/03/22 09:27:33 ngdp Exp $
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

    String get_filename () const;
    long get_lineno () const;
    void incrline ();

    void print_loc (ostream&) const;

    void set_filename (const String&);
    void set_lineno (long);

    /* public operators */

    Location& operator= (const Location&);

private:
    /* private state */

    String file;
    long lineno;

};

#include "Location.n"

#endif
