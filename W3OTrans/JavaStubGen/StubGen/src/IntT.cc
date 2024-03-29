/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IntT.cc,v 1.7 1995/03/14 09:11:47 ngdp Exp $
 */

/*
 * Stub generator integer type definition handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

static const char RCSid[] = "$Id: IntT.cc,v 1.7 1995/03/14 09:11:47 ngdp Exp $";

/*
 * PUBLIC operations
 */

IntType::IntType ()
		 : ModifiedType(INTEGRAL_TYPE),
		   intTypeName(GlobalInfo::getOutputStyle() == IDL_STYLE ? "long" : "int")
{
#ifdef SG_DEBUG
    debugStream << "IntType::IntType("  << (void *)this << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif 
}

IntType::~IntType ()
{
#ifdef SG_DEBUG
    debugStream << "IntType::~IntType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. We override that in ModifiedType
 * so that printing as IDL is simpler
 */

Boolean IntType::addTypeModifier ( TypeModifier newmod )
{
#ifdef SG_DEBUG
    debugStream << "IntType::addTypeModifier ("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newmod == MOD_LONG || newmod == MOD_SHORT)
    {
	if (GlobalInfo::getOutputStyle() == IDL_STYLE)
	{
	    /* IDL does not like 'int' */
	    
	    intTypeName = NullString;
	}
    }
    
    return ModifiedType::addTypeModifier(newmod);
}

TypeDescriptor *IntType::combineTypes ( TypeDescriptor *newT )
{
    if (newT->kind() == MODIFIED_TYPE)
	return newT->combineTypes(this);
    else
    {
	ostrstream errorStream;

	errorStream << "illegal type combination : " << typeName(NORMAL_DECL);
	errorStream << " " << newT->typeName(NORMAL_DECL);
	error(errorStream);

	return this;
    }
}

String IntType::signature () const
{
    return ModifiedType::signature() + "i";
}

String IntType::preName ( DeclStyle ds, Boolean elab,
			  Boolean noConst ) const
{
    String root = ModifiedType::preName(ds,elab, noConst);
    
    if (root != NullString)
	root += " ";

    root += intTypeName;
    
    return root;
}
