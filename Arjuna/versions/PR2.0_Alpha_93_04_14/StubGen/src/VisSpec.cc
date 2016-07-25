/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VisSpec.cc,v 1.4 1993/03/22 09:31:39 ngdp Exp $
 */

/*
 * Stub generator utility routines for handling VisibilitySpec variables
 *
 */

#include <strstream.h>

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

static const char RCSid[] = "$Id: VisSpec.cc,v 1.4 1993/03/22 09:31:39 ngdp Exp $";

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
