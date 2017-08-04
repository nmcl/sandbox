/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvQuals.h,v 1.1 1993/11/03 14:44:23 nmcl Exp $
 */

#ifndef CVQUALS_H_
#define CVQUALS_H_

/*
 *
 * Enumeration type describing qualifiers applicable to types.
 *     
 */


class ostream;

enum CvQualifier { QUAL_CONST = 1, QUAL_VOLATILE };

extern ostream& operator<< ( ostream&, CvQualifier );

#endif