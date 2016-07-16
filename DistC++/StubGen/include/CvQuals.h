/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvQuals.h,v 1.1 1997/09/25 15:29:44 nmcl Exp $
 */

#ifndef CVQUALS_H_
#define CVQUALS_H_

/*
 *
 * Enumeration type describing qualifiers applicable to types.
 *     
 */


#include <iostream>

using namespace std;

enum CvQualifier { QUAL_CONST = 1, QUAL_VOLATILE };

extern ostream& operator<< ( ostream&, CvQualifier );

#endif
