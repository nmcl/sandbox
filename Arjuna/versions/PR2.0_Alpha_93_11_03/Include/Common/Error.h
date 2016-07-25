/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.h,v 1.1 1993/11/03 14:28:25 nmcl Exp $
 */

#ifndef ERROR_H_
#define ERROR_H_

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

extern ostream& error_stream;

#include <Common/Error.n>

#endif

