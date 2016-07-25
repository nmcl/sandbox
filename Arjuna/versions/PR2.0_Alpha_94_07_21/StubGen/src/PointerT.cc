/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerT.cc,v 1.17 1993/08/16 14:04:22 ngdp Exp $
 */

/*
 * Stub generator pointer type handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
#endif

static const char RCSid[] = "$Id: PointerT.cc,v 1.17 1993/08/16 14:04:22 ngdp Exp $";

/*
 * PUBLIC operations
 */

PointerType::PointerType ( Type type, TypeDescriptor *base, 
			   TypeDescriptor *quals ) 
                         : TypeDescriptor(type),
			   pointerQuals(quals),
			   pointerTo(base)
{
#ifdef DEBUG
    debugStream << "PointerType::PointerType(" << (void *)this << "," 
		<< type << "," << (void *)base << "," << (void *)quals << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

PointerType::~PointerType ()
{
#ifdef DEBUG
    debugStream << "PointerType::~PointerType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean PointerType::addStubModifier ( StubModifier sm )
{
#ifdef DEBUG
    debugStream << "PointerType::addStubModifier("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if (pointerTo != 0)
	    pointerTo->addStubModifier(sm);
	return TRUE;
    default:
	break;
    }

    return TypeDescriptor::addStubModifier(sm);
}

Boolean PointerType::attachBaseType ( TypeDescriptor *baseType )
{
#ifdef DEBUG
    debugStream << "PointerType::attachBaseType(" << (void *)this 
		<< ", " << (void *)baseType << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (pointerTo)
	pointerTo->attachBaseType(baseType);
    else
	pointerTo = baseType;

    return TRUE;
}

/*
 * Adjust type of pointer. Sets pointerTo field, returns old value.
 * Used in building declarator type
 */

TypeDescriptor *PointerType::combineTypes ( TypeDescriptor *nt )
{
#ifdef DEBUG
    debugStream << "PointerType::combineTypes(" << (void *)this 
		<< ", " << (void *)nt << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (pointerTo)
	pointerTo->combineTypes(nt);
    else
	pointerTo = nt;

    return this;
}

TypeDescriptor *PointerType::deref () const
{
    return pointerTo;
}

/* Print default init for this type (ie = 0) */

String PointerType::defaultInit ( ) const
{
    if (kind() == POINTER_TYPE)
	return String(" = 0");
    else
	return NullString;
}


Boolean PointerType::isFundamental () const
{
    return FALSE;
}

String PointerType::postName ( DeclStyle ds ) const
{
    String s;
    
    if (pointerTo != 0)
    {
	Type pointedAt = pointerTo->kind();

	if ((pointedAt == VECTOR_TYPE) || (pointedAt == FUNCTION_TYPE))
	    s = ")";

	s += pointerTo->postName(ds);
    }
    return s;
}

String PointerType::preName ( DeclStyle ds, Boolean elab,
			      Boolean noConst ) const
{
    String s;
    
    if (pointerTo != 0)
    {	
	Type pointedAt = pointerTo->kind();

	s = pointerTo->preName(ds, elab, noConst);

	if ((pointedAt == VECTOR_TYPE) || (pointedAt == FUNCTION_TYPE))
	    s += "(";
    }

    if (kind() == POINTER_TYPE)
	s += "*";
    else
	s += "&";

    if (pointerQuals)
	s += pointerQuals->preName(ds, elab, noConst);
 
   return s;
}

String PointerType::signature () const
{
    String newname;
    String ptrname;

    if (kind() == POINTER_TYPE)
	ptrname = "P";
    else
	ptrname = "R";
    
    if (pointerTo != 0)
	newname = ptrname + pointerTo->signature();
    else
	newname = ptrname;

    if (pointerQuals)
	newname = newname + pointerQuals->signature();

    return newname;

}

StubModifier PointerType::defaultMarshall () const
{
    if (pointerTo != 0)
    {
	return pointerTo->defaultMarshall();
    }
    
    return STUB_NOMARSHALL;
}

StubModifier PointerType::defaultTransfer () const
{
    if (pointerTo != 0)
    {
	if (pointerTo->isConstant())
	    return STUB_ARG_IN;
    }
    
    return STUB_ARG_INOUT;
}

/*
 * Determine the (un)marshallability of the type being pointed at.
 * Note that we do not allow pointer to pointer (hence the fundamental check)
 */

Boolean PointerType::isTransferable () const
{
    if (pointerTo != 0)
    {
	if (pointerTo->isFundamental())
	    return pointerTo->isTransferable();

	/* Not pointing at fundamental type - may still be ok if I'm a ref */

	if (kind() == POINTER_TYPE)
	    return FALSE;
	else
	    return pointerTo->isTransferable();
    }
    else
	return FALSE;
}

/*
 * (Un)Marshalling of pointers and references is special in that
 * we attempt to detect if a pointer has been (un)packed already to avoid
 * packing (and unpacking) extra copies.
 * References to fundamental types (int etc.) are sent as if they were pointers
 * to ensure aliasing is handled correctly.
 * If the reference is to another pointer we simply send the pointer directly
 * in the normal manner.
 * When a pointer is packed / unpacked the name of the variable
 * has to be adjusted to take account of the indirection. These
 * routine handles this.
 */

Boolean PointerType::produceMarshallingCode ( ostream& s,
					      Direction d,
					      const String& varname,
					      const String& buffname )
{
    String newname = varname;

    if (d == IN)
    {
	if (kind() == REFERENCE_TYPE)
	{
	    if (pointerTo->isFundamental())
		newname = String("&") + varname;
	}
	return pointerTo->produceMarshallingCode(s, d, newname, buffname);
    }
    else
    {
	if ((kind() == REFERENCE_TYPE) && (pointerTo->isFundamental()))
	{
	    PointerType pt(POINTER_TYPE, pointerTo);
	    newname += "Ptr";
	    
	    pt.printInstanceOf(s, newname, NORMAL_DECL, FALSE);
	    s << " = &" << varname << ";\n\t";
	}
	return pointerTo->produceMarshallingCode(s, d, newname, buffname);
    }
}

 
