/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CharT.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $
 */

/*
 * Stub generator char type definition handler
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CHART_H_
#  include "CharT.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

static const char RCSid[] = "$Id: CharT.cc,v 1.1 1997/06/09 19:52:19 nmcl Exp $";

/*
 * PUBLIC operations
 */

CharType::CharType ( Boolean wideChar )
		   : ModifiedType(CHAR_TYPE),
		     wide(wideChar),
		     charTypeName(wide ? "wchar_t" : "char")
{
#ifdef SG_DEBUG
    debugStream << "CharType::CharType("  << (void *)this
		<< wideChar << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

CharType::~CharType ()
{
#ifdef SG_DEBUG
    debugStream << "CharType::~CharType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

/*
 * Adjust the modifier bits for the type. Fairly restricted for chars.
 */

Boolean CharType::addTypeModifier ( TypeModifier newmod )
{
#ifdef SG_DEBUG
    debugStream << "CharType::addTypeModifier ("  << (void *)this << ","
		<< newmod << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newmod == MOD_UNSIGNED || newmod == MOD_SIGNED)
    {
	if (GlobalInfo::getOutputStyle() == IDL_STYLE)
	{
	    if (newmod == MOD_UNSIGNED)
		charTypeName = "octet";
	}
	
	ModifiedType::addTypeModifier(newmod);
    }
    else
    {
	ostrstream errorStream;
	
	errorStream << "illegal type modifier in '";
	errorStream << charTypeName;
	errorStream << "'declaration";

	error(errorStream);
    }

    return TRUE;
}

TypeDescriptor *CharType::combineTypes ( TypeDescriptor *newT )
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

String CharType::signature () const
{
    return ModifiedType::signature() + "c";
}

String CharType::preName ( DeclStyle ds, Boolean elab, 
			   Boolean noConst ) const
{    
    String root;
    
    if ((GlobalInfo::getOutputStyle() != IDL_STYLE) &&
	(GlobalInfo::getOutputStyle() != JAVA_STYLE))
    {
	root = ModifiedType::preName(ds,elab, noConst);
    
	if (root != NullString)
	    root += " ";
    }
    
    root += charTypeName;
    
    return root;
}


