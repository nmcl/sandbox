/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QualT.cc,v 1.1 1993/11/03 14:48:09 nmcl Exp $
 */

/*
 * Stub generator qualified type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef QUALT_H_
#  include "QualT.h"
#endif

static const char RCSid[] = "$Id: QualT.cc,v 1.1 1993/11/03 14:48:09 nmcl Exp $";

/*
 * PUBLIC operations
 */

QualifiedType::QualifiedType ( TypeDescriptor *parent, TypeDescriptor *qt )
                             : TypeDescriptor(TYPE_TYPE),
			       qualifiedType(qt),
			       qualifyingType(parent)
{
#ifdef DEBUG
    debugStream << "QualifiedType::QualifiedType(" << (void *)this << ","
		<< (void *)parent << "," << (void *)qt << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

QualifiedType::~QualifiedType ()
{
#ifdef DEBUG
    debugStream << "QualifiedType::~QualifiedType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean QualifiedType::addBody ( Expression* body )
{
    if (qualifiedType != 0)
    {
	return qualifiedType->addBody(body);
    }
    else
	return TypeDescriptor::addBody(body);
    
}

Boolean QualifiedType::addFuncModifier ( FuncModifier newFm )
{
#ifdef DEBUG
    debugStream << "QualifiedType::addFuncModifier("  << (void *)this
		<< "," << newFm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (qualifiedType != 0)
    {
	return qualifiedType->addFuncModifier(newFm);
    }
    else
	return TypeDescriptor::addFuncModifier(newFm);
}

TypeDescriptor *QualifiedType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debugStream << "QualifiedType::combineTypes ("  << (void *)this << ","
		<< (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (qualifiedType == 0)
    {
	qualifiedType = newtype;
    }
    else
	qualifiedType = combineTypes(newtype);

    return this;
}

TypeDescriptor *QualifiedType::deref () const
{
    if (qualifiedType == 0)
	return 0;

    if (qualifiedType->kind() != TYPE_TYPE)
	return qualifiedType;
    else
	return qualifiedType->deref();
}

String QualifiedType::massageName ( const String&, DeclStyle ) const
{    
    return NullString;
}

String QualifiedType::postName ( DeclStyle ds ) const
{
    String s = NullString;

    if (qualifyingType != 0)
	s = qualifiedType->postName(ds);
    
    if (qualifiedType != 0)
	s += qualifiedType->postName(ds);

    return s;
}

String QualifiedType::preName ( DeclStyle ds, Boolean elab,
				Boolean noConst ) const
{
    String s = NullString;

    if (qualifyingType != 0)
	s = qualifiedType->preName(ds,elab,noConst);
    
    s += "::";
    
    if (qualifiedType != 0)
	s += qualifiedType->preName(ds,elab,noConst);

    return s;
}

String QualifiedType::signature () const
{
    String sig = NullString;

    if (qualifyingType != 0)
	sig = qualifyingType->signature();

    if (qualifiedType != 0)
	sig += qualifiedType->signature();

    return sig;
}

Boolean QualifiedType::isTransferable () const
{
    if (qualifiedType != 0)
	return qualifiedType->isTransferable ();
    else
	return FALSE;
}

Boolean QualifiedType::produceMarshallingCode ( ostream& s,
						Direction d,
						const String& vn,
						const String& bn )
{
    if (qualifiedType)
	return qualifiedType->produceMarshallingCode(s,d,vn,bn);
    return FALSE;
}

ostream& QualifiedType::printAsTypedef ( ostream& s, DeclStyle ds )
{
    if (qualifiedType != 0)
	qualifiedType->printAsTypedef(s, ds);
    return s;
}
