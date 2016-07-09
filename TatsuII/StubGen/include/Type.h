/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Type.h,v 1.1 1997/06/09 19:52:18 nmcl Exp $
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <iostream>

using namespace std;

/*
 *
 * Enumeration type describing basic kinds of types that the stub
 * generator will create.
 * Whether its good practice or not we often need to know what kind
 * of object is being manipulated in the parser this enum helps figure that
 * out simply:
 *
 * AGGREGATE = class/struct/union
 * INTEGRAL  = char/int etc.
 * TYPE      = typedef
 * NONE      = other stuff (used for include file names for example which
 *			    are placed in symbol tables as funny declarations)
 * ANY       = used in lookup routines where no particular type of declaration
 *             is wanted.
 *     
 * BEWARE this list is ordered deliberately!
 */


enum Type { NONE_TYPE = 0,
	    AGGREGATE_TYPE = 0x0001, MODIFIED_TYPE = 0x0002,
	    INTEGRAL_TYPE = 0x0004, FLOATING_TYPE = 0x0008,
	    VOID_TYPE = 0x0010, ENUMERATION_TYPE = 0x0020,
	    REFERENCE_TYPE = 0x0040, POINTER_TYPE = 0x0080,
	    FUNCTION_TYPE = 0x0100, VECTOR_TYPE = 0x0200,
	    TYPE_TYPE = 0x0400, CV_TYPE = 0x0800,
	    CHAR_TYPE = 0x1000, ANY_TYPE = 0xffff };

extern ostream& operator<< (ostream&, Type);

#endif
