/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.cc,v 1.4 1993/03/18 14:17:29 nsmw Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

static const char RCSid[] = "$Id: Error.cc,v 1.4 1993/03/18 14:17:29 nsmw Exp $";

ostream& error_stream = cerr;

#ifdef NO_INLINES
#  define ERROR_CC_
#  include <Common/Error.n>
#  undef ERROR_CC_
#endif
