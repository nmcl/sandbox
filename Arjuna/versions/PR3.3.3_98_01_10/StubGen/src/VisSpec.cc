/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VisSpec.cc,v 1.1 1993/11/03 14:48:29 nmcl Exp $
 */

/*
 * Stub generator utility routines for handling VisibilitySpec variables
 *
 */

#include <strstream.h>

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

static const char RCSid[] = "$Id: VisSpec.cc,v 1.1 1993/11/03 14:48:29 nmcl Exp $";

ostream& operator<< ( ostream& s, VisibilitySpec vs )
{
    if (vs == CLASS_PRIVATE)
	s << "CLASS_PRIVATE";
    else if (vs == CLASS_PROTECTED)
	s << "CLASS_PROTECTED";
    else if (vs == CLASS_PUBLIC)
	s << "CLASS_PUBLIC";
    else if (vs == CLASS_DEFAULT)
	s << "CLASS_DEFAULT";
    else if (vs == FILE_STATIC)
	s << "FILE_STATIC";
    else
	s << "FILE_EXTERN";

    return s;
}
