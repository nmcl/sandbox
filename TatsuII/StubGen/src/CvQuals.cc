/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvQuals.cc,v 1.1 1997/06/09 19:52:20 nmcl Exp $
 */

/*
 * Stub generator utility routines for CvQualifier variables
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef CVQUALS_H_
#  include "CvQuals.h"
#endif

static const char RCSid[] = "$Id: CvQuals.cc,v 1.1 1997/06/09 19:52:20 nmcl Exp $";

ostream& operator<< ( ostream& s, CvQualifier cvq )
{
    if (cvq == QUAL_CONST)
	s << "const ";
    else
	s << "volatile ";

    return s;
}
