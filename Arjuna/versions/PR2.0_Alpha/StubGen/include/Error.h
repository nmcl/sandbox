/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.6 1993/03/22 09:27:14 ngdp Exp $
 */

#ifndef ERROR_H_
#define ERROR_H_

/*
 *
 * Stub generator error handling. 
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef LOCATION_H_
#  include "Location.h"
#endif

enum ErrorType {WARNING, ERROR, FATAL};

class Error;
class ostream;				/* avoid #include <iostream.h> */
class ostrstream;
class Declaration;			/* likewise for Declaration.h */

extern void error (char *);
extern void warning (char *);
extern void fatal (char *);

extern Error error_log;

extern Location error_location;
extern ostrstream& error_stream;

class Error
{
public:
    Error ();

    int no_of_errors () const;
    void do_warnings ();
    void post_error (ostrstream&, ErrorType);
    void summary (ostream&);

private:
    int errorcount;
    int warningcount;
    Boolean warnings_wanted;
};

#include "Error.n"

#endif
