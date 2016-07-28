/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FloatT.cc,v 1.4 1995/03/14 09:11:40 ngdp Exp $
 */

/*
 * Stub generator float type definition handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef FLOATT_H_
#  include "FloatT.h"
#endif

static const char RCSid[] = "$Id: FloatT.cc,v 1.4 1995/03/14 09:11:40 ngdp Exp $";
/*
 * PUBLIC operations
 */

FloatType::FloatType ( Boolean doubleType ) 
                     : ModifiedType(FLOATING_TYPE),
		       isDouble(doubleType)
{
#ifdef SG_DEBUG
    debugStream << "FloatType::FloatType(" << (void *)this
		<< "," << doubleType << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

FloatType::~FloatType ()
{
#ifdef SG_DEBUG
    debugStream << "FloatType::~FloatType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. Eg long or short etc. Mostly
 * meaningless for floats.
 */

Boolean FloatType::addTypeModifier ( TypeModifier newmod )
{
#ifdef SG_DEBUG
    debugStream << "FloatType::addTypeModifier ("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);

#endif

    if (newmod == MOD_LONG)
	return ModifiedType::addTypeModifier(newmod);
    else
	error("illegal type modifier in 'float' declaration");
    return FALSE;
}

TypeDescriptor *FloatType::combineTypes ( TypeDescriptor *newT )
{
    if (newT->kind() == MODIFIED_TYPE)
	return newT->combineTypes(this);
    else
    {
	ostrstream errorStream;

	errorStream << "illegal type specification : " << typeName(NORMAL_DECL);
	errorStream << " " << newT->typeName(NORMAL_DECL);
	error(errorStream);

	return this;
    }
}

String FloatType::preName ( DeclStyle ds, Boolean elab, 
			    Boolean noConst ) const
{
    String s = ModifiedType::preName(ds,elab,noConst);
    
    if (s != NullString)
	s += " ";
    
    if (isDouble)
	s += "double";
    else
	s += "float";

    return s;
}

String FloatType::signature () const
{
    if (isDouble)
	return ModifiedType::signature() + "d";
    else
	return ModifiedType::signature() + "f";
}


