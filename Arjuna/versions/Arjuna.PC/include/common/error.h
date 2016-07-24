/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.2 1994/01/06 12:13:23 ngdp Exp $
 */

#ifndef ERROR_H_
#define ERROR_H_

#if defined(OS2) || defined(MSDOS)
#  ifndef IOSTREAM_H_
#    include <System/iostream.h>
#  endif
#endif


/*
 *
 * Error reporting routines
 *
 */

class ostream;

enum ErrorSeverity
{
    FATAL, WARNING
};

extern void ArjunaFatal ();

extern ostream& operator<< ( ostream& strm, enum ErrorSeverity es );

#define error_stream cerr

#include <Common/Error.n>

#endif

