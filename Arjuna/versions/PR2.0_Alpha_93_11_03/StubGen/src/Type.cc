/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Type.cc,v 1.1 1993/11/03 14:48:22 nmcl Exp $
 */

/*
 * Stub generator utility routines for Type variables
 *
 */

#include <strstream.h>

#ifndef TYPE_H_
#  include "Type.h"
#endif

static const char RCSid[] = "$Id: Type.cc,v 1.1 1993/11/03 14:48:22 nmcl Exp $";

ostream& operator<< ( ostream& s, Type t )
{
    switch (t)
    {
    case AGGREGATE_TYPE:
	s << "AGGREGATE_TYPE";
	break;
    case INTEGRAL_TYPE:
	s << "INTEGRAL_TYPE";
	break;
    case FLOATING_TYPE:
	s << "FLOATING_TYPE";
	break;
    case VOID_TYPE:
	s << "VOID_TYPE";
	break;
    case ENUMERATION_TYPE:
	s << "ENUMERATION_TYPE";
	break;
    case REFERENCE_TYPE:
	s << "REFERENCE_TYPE";
	break;
    case POINTER_TYPE:
	s << "POINTER_TYPE";
	break;
    case FUNCTION_TYPE:
	s << "FUNCTION_TYPE";
	break;
    case VECTOR_TYPE:
	s << "VECTOR_TYPE";
	break;
    case TYPE_TYPE:
	s << "TYPE_TYPE";
	break;
    case CV_TYPE:
	s << "CV_TYPE";
	break;
    case NONE_TYPE:
	s << "NONE_TYPE";
	break;
    case ANY_TYPE:
	s << "ANY_TYPE";
    }

    return s;
}

