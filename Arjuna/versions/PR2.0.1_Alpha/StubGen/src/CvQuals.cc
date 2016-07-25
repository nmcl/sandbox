/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvQuals.cc,v 1.1 1993/11/03 14:47:18 nmcl Exp $
 */

/*
 * Stub generator utility routines for CvQualifier variables
 *
 */

#include <strstream.h>

#ifndef CVQUALS_H_
#  include "CvQuals.h"
#endif

static const char RCSid[] = "$Id: CvQuals.cc,v 1.1 1993/11/03 14:47:18 nmcl Exp $";

ostream& operator<< ( ostream& s, CvQualifier cvq )
{
    if (cvq == QUAL_CONST)
	s << "const ";
    else
	s << "volatile ";

    return s;
}
