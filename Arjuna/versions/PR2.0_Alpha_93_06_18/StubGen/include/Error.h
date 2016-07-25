/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.9 1993/09/20 14:09:13 ngdp Exp $
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

enum ErrorType {WARNING, CAUTION, ERROR, FATAL};

class Error;
class ostream;				/* avoid #include <iostream.h> */
class ostrstream;
class Declaration;			/* likewise for Declaration.h */

extern void error (char *);
extern void caution (char *);
extern void warning (char *);
extern void fatal (char *);

extern Error errorLog;

extern Location errorLocation;
extern ostrstream& errorStream;

class Error
{
public:
    Error ();

    void enableWarnings (Boolean);
    int noOfErrors () const;
    void postError (ostrstream&, ErrorType);
    void summary (ostream&);
    void supressWarnings (Boolean);
    Boolean warningsEnabled () const;

private:
    int errorCount;
    int warningCount;
    Boolean warningsWanted;
    Boolean warningsSupressed;
};

#include "Error.n"

#endif
