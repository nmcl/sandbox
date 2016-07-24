/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Error.cc,v 1.1 1993/11/03 14:15:00 nmcl Exp $
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

static const char RCSid[] = "$Id: Error.cc,v 1.1 1993/11/03 14:15:00 nmcl Exp $";

ostream& error_stream = cerr;

#ifdef NO_INLINES
#  define ERROR_CC_
#  include <Common/Error.n>
#  undef ERROR_CC_
#endif
