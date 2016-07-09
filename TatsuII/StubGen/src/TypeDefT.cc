/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TypeDefT.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $
 */

/*
 * Stub generator typedef definition handler
 *
 */

#include <fstream>
#include <strstream>

#ifdef SG_DEBUG
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

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef INTERFACE_H_
#  include "Interface.h"
#endif

static const char RCSid[] = "$Id: TypeDefT.cc,v 1.1 1997/06/09 19:52:24 nmcl Exp $";

/*
 * PUBLIC operations
 */

TypeDefType::TypeDefType ( const String& name, TypeDescriptor *td )
                         : TypeDescriptor(TYPE_TYPE),
			   realType(td),
			   typedefName(name)
{
#ifdef SG_DEBUG
    debugStream << "TypeDefType::TypeDefType(" << (void *)this << ","
		<< name << "," << (void *)td << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDefType::~TypeDefType ()
{
#ifdef SG_DEBUG
    debugStream << "TypeDefType::~TypeDefType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean TypeDefType::addStubModifier ( StubModifier sm )
{
    if (realType !=0 )
	return realType->addStubModifier(sm);
    else
	return TypeDescriptor::addStubModifier(sm);
}

TypeDescriptor *TypeDefType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef SG_DEBUG
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

TypeDescriptor *TypeDefType::deref ()
{
    if (realType == 0)
	return this;

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

Boolean TypeDefType::deleteable ( ) const
{
    if (realType != 0)
	return realType->deleteable();
    else
	return TypeDescriptor::deleteable();
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

String TypeDefType::preName ( DeclStyle ds , Boolean elab, Boolean ) const
{
    String name;

    if (elab)
	name = realType->elaborator(ds);

    name += typedefName;

    return name;
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

Boolean TypeDefType::produceAllStubs ( ostream& hdr )
{
    printAsTypedef(hdr, NORMAL_DECL);
    
    hdr << ";\n";

    return produceMarshallDefs(hdr, NORMAL_DECL);
}


Boolean TypeDefType::produceMarshallDefs ( ostream& hdr, DeclStyle ds, Boolean full )
{    
    if (realType)
	realType->produceMarshallDefs(hdr,ds,full);
    
    return TRUE;
}

Boolean TypeDefType::produceMarshallingCode ( ostream& s,
					      DeclStyle ds,
					      Direction d,
					      const String& varname, 
					      const String& buffname) 
{  
    if (realType)
	realType->produceMarshallingCode(s,ds,d,varname,buffname);
    
    return TRUE;
}

ostream& TypeDefType::printAsTypedef ( ostream& s, DeclStyle ds )
{
    s << "typedef ";
    
    s << realType->preName(ds);
    s << " " << typedefName;
    s << realType->postName(ds);
    return s;
}

Boolean TypeDefType::stubIsPossible () const
{
    return TRUE;
}
