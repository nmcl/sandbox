/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvQuals.cc,v 1.1.6.1 1996/10/10 12:26:50 ngdp Exp $
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

static const char RCSid[] = "$Id: CvQuals.cc,v 1.1.6.1 1996/10/10 12:26:50 ngdp Exp $";

ostream& operator<< ( ostream& s, CvQualifier cvq )
{
    if (cvq == QUAL_CONST)
	s << "const ";
    else
	s << "volatile ";

    return s;
}
