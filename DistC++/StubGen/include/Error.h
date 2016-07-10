/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.1 1997/09/25 15:29:58 nmcl Exp $
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

extern void error (const char *);
extern void caution (const char *);
extern void warning (const char *);
extern void fatal (const char *);

class Error;
class Declaration;			/* likewise for Declaration.h */

extern Error errorLog;
extern Location errorLocation;

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
    int supressedCount;
    Boolean warningsWanted;
    Boolean warningsSupressed;
};

#include "Error.n"

#endif
