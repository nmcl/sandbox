/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.cc,v 1.13 1993/08/16 14:04:26 ngdp Exp $
 */

/*
 * Stub generator typedef definition handler
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

#ifndef TYPEDEFT_H_
#  include "TypeDefT.h"
#endif

static const char RCSid[] = "$Id: TypeDefT.cc,v 1.13 1993/08/16 14:04:26 ngdp Exp $";

/*
 * PUBLIC operations
 */

TypeDefType::TypeDefType ( const String& name, TypeDescriptor *td )
                         : TypeDescriptor(TYPE_TYPE),
			   realType(td),
			   typedefName(name)
{
#ifdef DEBUG
    debugStream << "TypeDefType::TypeDefType(" << (void *)this << ","
		<< name << "," << (void *)td << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDefType::~TypeDefType ()
{
#ifdef DEBUG
    debugStream << "TypeDefType::~TypeDefType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor *TypeDefType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debugStream << "TypeDefType::combineTypes ("  << (void *)this << ","
		<< (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (realType == 0)
    {
	realType = newtype;
    }
    else
	realType = combineTypes(newtype);

    return this;
}

TypeDescriptor *TypeDefType::deref () const
{
    if (realType == 0)
	return 0;

    if (realType->kind() != TYPE_TYPE)
	return realType;
    else
	return realType->deref();
}

String TypeDefType::defaultInit ( ) const
{
    if (realType != 0)
	return realType->defaultInit();
    else
	return NullString;
}

Boolean TypeDefType::isFundamental () const
{
    if (realType != 0)
	return realType->isFundamental();
    else
	return FALSE;
}

String TypeDefType::postName ( DeclStyle ) const
{
    return NullString;
}

String TypeDefType::preName ( DeclStyle, Boolean, Boolean ) const
{
    return typedefName;
}

String TypeDefType::signature () const
{
    return typedefName;    
}

String TypeDefType::simpleTypeName ( DeclStyle ) const
{
    return typedefName;
}

StubModifier TypeDefType::defaultMarshall () const
{
    if (realType != 0)
	return realType->defaultMarshall();
    else
	return STUB_NOMARSHALL;
}

StubModifier TypeDefType::defaultTransfer () const
{
    if (realType != 0)
	return realType->defaultTransfer();
    else
	return STUB_ARG_IN;
}

Boolean TypeDefType::isTransferable () const
{
    if (realType != 0)
	return realType->isTransferable();
    else
	return FALSE;
}

Boolean TypeDefType::produceMarshallingCode ( ostream& s,
					      Direction d,
					      const String& varname, 
					      const String& buffname) 
{  
    if (realType)
	realType->produceMarshallingCode(s,d,varname,buffname);
    
    return TRUE;
}

ostream& TypeDefType::printAsTypedef ( ostream& s, DeclStyle ds )
{
    /* 'typedef' printed as storage class in DeclSpecifier */

    s << realType->preName(ds);
    s << " " << typedefName;
    s << realType->postName(ds);
    return s;
}

