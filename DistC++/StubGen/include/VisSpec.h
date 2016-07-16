/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VisSpec.h,v 1.1 1997/09/25 15:30:33 nmcl Exp $
 */

#ifndef VISSPEC_H_
#define VISSPEC_H_

/*
 *
 * Enumeration type describing visibility of class members.
 *     
 */

#include <iostream>

using namespace std;


enum VisibilitySpec { CLASS_PRIVATE = 1, CLASS_PROTECTED, CLASS_PUBLIC,
		      CLASS_DEFAULT, FILE_STATIC, FILE_EXTERN };

extern ostream& operator<< ( ostream&, VisibilitySpec );

#endif
