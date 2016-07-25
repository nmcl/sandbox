/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: VecT.cc,v 1.15 1993/08/16 14:04:27 ngdp Exp $
 */

/*
 * Stub generator vector (array) type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef VECT_H_
#  include "VecT.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

static const char RCSid[] = "$Id: VecT.cc,v 1.15 1993/08/16 14:04:27 ngdp Exp $";

/*
 * PUBLIC operations
 */

VecType::VecType ( Expression *vs, TypeDescriptor *isOf )
                 : TypeDescriptor(VECTOR_TYPE),
		   vectorSize(vs),
		   vectorOf(isOf)
{
#ifdef DEBUG
    debugStream << "VecType::VecType("  << (void *)this << ","
		<< vs << ", " << (void *)isOf << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

VecType::~VecType ()
{
#ifdef DEBUG
    debugStream << "VecType::~VecType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean VecType::attachBaseType ( TypeDescriptor *baseType )
{
#ifdef DEBUG
    debugStream << "VecType::attachBaseType(" << (void *)this 
		<< ", " << (void *)baseType << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (vectorOf)
	vectorOf->attachBaseType(baseType);
    else
	vectorOf = baseType;

    return TRUE;
}

/*
 * Adjust type of vector. Sets vectorOf field, returns old value.
 * Used in building declarator type
 */

TypeDescriptor *VecType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debugStream << "VecType::combineTypes ("  << (void *)this << ","
	         << (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (vectorOf)
	vectorOf->combineTypes(newtype);
    else
	vectorOf = newtype;

    return this;
}

String VecType::postName ( DeclStyle ds ) const
{
    String s = "[";

    if (vectorSize != 0)
#ifndef __GNUG__
        s += *vectorSize;
#else
// g++ does not like conversion operators.
        s += vectorSize->asString();
#endif

    s += "]";

    if (vectorOf != 0)
        s += vectorOf->postName(ds);

    return s;
}

String VecType::preName ( DeclStyle ds, Boolean elab, Boolean noConst ) const
{
    if (vectorOf != 0)
	return vectorOf->preName(ds,elab,noConst);
    else
	return NullString;
}

Boolean VecType::isFundamental () const
{
    return FALSE;
}

String VecType::signature () const
{
    String sig;

    sig += "A_";

    if (vectorOf != 0)
        sig += vectorOf->signature();

    return sig;    
}

StubModifier VecType::defaultMarshall () const
{
    if (vectorOf != 0)
	return vectorOf->defaultMarshall();
    else
	return STUB_NOMARSHALL;
}

StubModifier VecType::defaultTransfer () const
{
    return STUB_ARG_INOUT;
}

Boolean VecType::isTransferable () const
{
    if (vectorOf)
	return vectorOf->isTransferable();
    else
	return TRUE;
}

Boolean VecType::produceMarshallingCode ( ostream& s,
					  Direction d,
					  const String& varname, 
					  const String& buffname) 
{
    String indexvarName = indexName();
    String newstring = varname + "[" + indexvarName + "]";

    s << "\t";
    s << "for (int " << indexvarName << " = 0; ";
    s << indexvarName << " < " << *vectorSize << "; ";
    s << indexvarName << "++ )\n";
    return vectorOf->produceMarshallingCode(s, d, newstring, buffname);
}

/*
 * PRIVATE operations
 */

String VecType::indexName ()
{
    static long index_number;           /* ensure each index is unique */

    return constructName("i_", index_number++);
}
