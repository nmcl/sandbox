/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QualT.cc,v 1.6 1995/06/23 08:44:19 ngdp Exp $
 */

/*
 * Stub generator qualified type definition handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
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

static const char RCSid[] = "$Id: QualT.cc,v 1.6 1995/06/23 08:44:19 ngdp Exp $";

/*
 * PUBLIC operations
 */

QualifiedType::QualifiedType ( TypeDescriptor *parent, TypeDescriptor *qt )
                             : TypeDescriptor(TYPE_TYPE),
			       qualifiedType(qt),
			       qualifyingType(parent)
{
#ifdef SG_DEBUG
    debugStream << "QualifiedType::QualifiedType(" << (void *)this << ","
		<< (void *)parent << "," << (void *)qt << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

QualifiedType::~QualifiedType ()
{
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
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

Boolean QualifiedType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "QualifiedType::addStubModifier("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (qualifiedType)
	return qualifiedType->addStubModifier(sm);
    else
	return TypeDescriptor::addStubModifier(sm);
}

TypeDescriptor *QualifiedType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef SG_DEBUG
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

TypeDescriptor *QualifiedType::deref ()
{
    if (qualifiedType == 0)
	return this;

    if (qualifiedType->kind() != TYPE_TYPE)
	return qualifiedType;
    else
	return qualifiedType->deref();
}

String QualifiedType::massageName ( const String& name, DeclStyle ds ) const
{
    if (qualifiedType)
	return qualifiedType->massageName(name,ds);
    else
	return name;
}

String QualifiedType::postName ( DeclStyle ds ) const
{
    String s = NullString;

    if (qualifyingType != 0)
	s = qualifyingType->postName(ds);
    
    if (qualifiedType != 0)
	s += qualifiedType->postName(ds);

    return s;
}

String QualifiedType::preName ( DeclStyle ds, Boolean elab,
				Boolean noConst ) const
{
    String s = NullString;

    if (qualifyingType != 0)
	s = qualifyingType->preName(ds,elab,noConst);

    if (ds == JAVA_DECL)
	s += ".";
    else
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
						DeclStyle ds,
						Direction d,
						const String& vn,
						const String& bn )
{
    if (qualifiedType)
	return qualifiedType->produceMarshallingCode(s,ds,d,vn,bn);
    return FALSE;
}

ostream& QualifiedType::printAsTypedef ( ostream& s, DeclStyle ds )
{
    if (qualifiedType != 0)
	qualifiedType->printAsTypedef(s, ds);
    return s;
}
