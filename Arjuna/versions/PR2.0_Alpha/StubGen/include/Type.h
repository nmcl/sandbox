/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Type.h,v 1.5 1993/03/22 09:27:47 ngdp Exp $
 */

#ifndef TYPE_H_
#define TYPE_H_

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


enum Type { AGGREGATE_TYPE = 1, INTEGRAL_TYPE, FLOATING_TYPE, VOID_TYPE,
	    ENUMERATION_TYPE, REFERENCE_TYPE, POINTER_TYPE, FUNCTION_TYPE,
	    VECTOR_TYPE, TYPE_TYPE, CV_TYPE, NONE_TYPE, ANY_TYPE };

class ostream;

extern ostream& operator<< (ostream&, Type);

#endif
