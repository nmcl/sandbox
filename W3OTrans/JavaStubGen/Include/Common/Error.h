/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.12 1995/03/07 14:59:21 ngdp Exp $
 */

#ifndef ERROR_H_
#define ERROR_H_

/*
 * WARNING:
 * Do not move the following #define
 */

#define error_stream ((ostream&)cerr)

#ifndef STUB
#  include <Common/Error_stub.h>
#else

/*
 *
 * Error reporting routines
 *
 */

#ifndef FACCODE_H_
#  include <Common/FacCode.h>
#endif

#ifndef ERRCODES_H_
#  include <Common/ErrCodes.h>
#endif

enum ErrorSeverity
{
    FATAL, WARNING
};

class ostream;

// @NoRemote
class Error
{
    friend class ErrorList;
    friend class ErrorListI;
    
public:
    Error (FacilityCode, int, const char* const, ErrorSeverity = WARNING);
    Error ();
    ~Error ();

    int errorCode () const;
    FacilityCode errorFac () const;
    operator const char *const () const;
    
    ostream& print (ostream&) const;
    
private:
    
    /* List chaining routines for Errors */

    Error *getLink () const;
    void setLink (Error *pointTo);

    int errCode;
    FacilityCode errorFacility;
    ErrorSeverity errorLevel;
    char *errorHead;
    char *errorText;

    Error *nextError;
};

extern ostream& operator<< (ostream&, ErrorSeverity);
extern void ArjunaFatal ();

#include <Common/Error.n>

#endif
#endif
