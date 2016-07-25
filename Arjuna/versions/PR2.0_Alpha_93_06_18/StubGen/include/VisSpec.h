/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VisSpec.h,v 1.4 1993/03/22 09:27:54 ngdp Exp $
 */

#ifndef VISSPEC_H_
#define VISSPEC_H_

/*
 *
 * Enumeration type describing visibility of class members.
 *     
 */


class ostream;				/* don't want to include iostream.h */

enum VisibilitySpec { CLASS_PRIVATE = 1, CLASS_PROTECTED, CLASS_PUBLIC,
		      CLASS_DEFAULT, FILE_STATIC, FILE_EXTERN };

extern ostream& operator<< ( ostream&, VisibilitySpec );

#endif
