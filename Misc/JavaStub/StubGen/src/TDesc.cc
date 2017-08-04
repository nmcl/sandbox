/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TDesc.cc,v 1.13 1995/07/03 14:30:01 ngdp Exp $
 */

/*
 * Stub generator basic type attribute class. The operations here provide
 * default implementations for the operations on the type description 
 * hierarchy rooted on the class TypeDescriptor. Many are simple no-ops
 * or have trivial semantics. More complicated behaviour should be 
 * implemented in the appropriate derived class.
 * Some operations are defined pure virtual to force redefinition - mostly
 * those for which a reasonable default does not exist.
 * We do not make these inline since they are all virtual and cannot be
 * inlined as a general rule.
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif
     
static const char RCSid[] = "$Id: TDesc.cc,v 1.13 1995/07/03 14:30:01 ngdp Exp $";
     
/*
 * Public operations
 */

/*
 * Walk type descriptor (using deref) stripping off types described
 * in the mask
 */

TypeDescriptor *TypeDescriptor::simplify ( int mask )
{
    TypeDescriptor *next = this;

    while ((next != 0) &&
	   ((next->kind() & mask) != 0) &&
	   (next = next->deref()))
    {
	if (next == this)
	    break;

    }

    return next;
}

/* 
 * Type manipulation routines.
 * These mangle types appropriately as more information is gathered by
 * the parser
 */

/*
 * Operation to deal with function bodies for declarations. Called when
 * inline body is added.
 */

Boolean TypeDescriptor::addBody ( Expression * )
{
    error("Function body illegal in this context");
    return FALSE;
}

/*
 * Operation to deal with type qualifier (CONST etc) If we reach here
 * then the qualifier is illegal in this context. Say so
 */

Boolean TypeDescriptor::addCvQualifier ( CvQualifier cvq )
{
    ostrstream errorStream;

    errorStream << "illegal type qualifier '" << cvq << "'in declaration";
    error(errorStream);
    return FALSE;
}
     
/*
 * Operation to deal with function type modifiers (INLINE etc) If we reach here
 * then the modifier is illegal in this context. Say so
 */

Boolean TypeDescriptor::addFuncModifier ( FuncModifier fm )
{
    ostrstream errorStream;

    errorStream << "illegal function modifier '" << fm << "'in declaration";
    error(errorStream);
    return FALSE;
}

/*
 * Default to no-op as normally basic types do not have names. Not true
 * for classes and functions
 */

Boolean TypeDescriptor::addName ( const String& )
{
    return TRUE;
}

/*
 * Operation to deal with stub specific modifiers.  If we reach here
 * then the modifier is generally illegal in this context. We ignore
 * 'harmless' modifiers such as NoRemote since they get set implicitly
 * for class members
 */

Boolean TypeDescriptor::addStubModifier ( StubModifier sm )
{
    ostrstream errorStream;

    switch (sm)
    {
    case STUB_DEFAULT:
    case STUB_PENDING:
	break;
    default:
	errorStream << "stub modifier '" << sm 
		    << "' illegal in this declaration";
	error(errorStream);
	return FALSE;
    }
    return TRUE;
}

/*
 * Operation to deal with type modifiers (SHORT etc) If we reach here
 * then the modifier is illegal in this context. Say so
 */

Boolean TypeDescriptor::addTypeModifier ( TypeModifier tm )
{
    ostrstream errorStream;

    errorStream << "illegal type modifier '" << tm << "'in declaration";
    error(errorStream);
    return FALSE;
}

/*
 * This routine is used by the process that attaches the basic delarator
 * type (eg int) to a declaration.
 */

Boolean TypeDescriptor::attachBaseType ( TypeDescriptor * )
{
    return TRUE;
}

/*
 * Typically only valid for pointers so return 'this' as a default
 */

TypeDescriptor *TypeDescriptor::deref ()
{
    return this;
}

String TypeDescriptor::elaborator ( DeclStyle ) const
{
    return NullString;
}

Boolean TypeDescriptor::freezeType ()
{
    return TRUE;
}

Boolean TypeDescriptor::makeComplete ()
{
    return TRUE;
}

Boolean TypeDescriptor::overrideStubModifiers ( StubModifier newSm )
{
    return addStubModifier(newSm);
}

void TypeDescriptor::setContext ( DeclContext *c )
{
#ifdef SG_DEBUG
    debugStream << "TypeDescriptor::setContext("  << (void *)this
		<< ", " << (void *)c << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

 
    declaredIn = c;
}

/* 
 * Type interrogation 
 */

Boolean TypeDescriptor::canDiscard () const
{
    return FALSE;
}

/* Default init for this type (ie = 0) */

String TypeDescriptor::defaultInit ( ) const
{
    return NullString;
}

/* Can we say delete on these types? */

Boolean TypeDescriptor::deleteable () const
{
    return FALSE;
}

DeclContext *TypeDescriptor::getContext () const
{
    return declaredIn;
}

/*
 * To optimise param passing we need to check if a parameter is const
 * or not so that the copy out can be avoided
 */

Boolean TypeDescriptor::isConstant () const
{
    return FALSE;
}

/*
 * Is this type a fundamental one (int, char, class etc)
 */

Boolean TypeDescriptor::isFundamental () const
{
    return TRUE;
}

/*
 * This could be a protected data member but I try never to export data
 * members to anyone - even derived classes. Overhead not high in this
 * case anyway.
 */

Type TypeDescriptor::kind () const
{
    return isKindOf;
}

/*
 * Declaration name massaging. OrigName is that given in the declaration
 * This operation mangles that name as approprite. Here we just return
 * the given name unchanged.
 */

String TypeDescriptor::massageName ( const String& origName,
				     DeclStyle ) const
{
    return origName;
}

/*
 * Do declarations involving this type need trailing separator 
 */

Boolean TypeDescriptor::needsSep () const
{
    return TRUE;
}

/*
 * Type name minus qualifiers (const volatile etc)
 */

String TypeDescriptor::simpleTypeName ( DeclStyle ds ) const
{
    char buffer[1024];
    ostrstream tempStream(buffer, 1024);
    
    tempStream << preName(ds, FALSE, TRUE);
    tempStream << postName(ds);

    tempStream << ends;
    
    return buffer;

}

String TypeDescriptor::typeName ( DeclStyle ds,
				  Boolean elaborated,
				  Boolean ) const
{
    char buffer[1024];
    ostrstream tempStream(buffer, 1024);
    
    tempStream << preName(ds, elaborated, FALSE);
    tempStream << postName(ds);
    
    tempStream << ends;
    
    return buffer;
}

/*
 * Stub generation default attribute handling. 
 */

StubModifier TypeDescriptor::defaultLocation () const
{
    return STUB_NOREMOTE;
}

StubModifier TypeDescriptor::defaultMarshall () const
{
    return STUB_NOMARSHALL;
}

StubModifier TypeDescriptor::defaultTransfer () const
{
    return STUB_ARG_NEITHER;
}

Boolean TypeDescriptor::produceAllStubs ( ostream& ) 
{
    return TRUE;
}
    
Boolean TypeDescriptor::produceMarshallingCode ( ostream& s,
						 DeclStyle ds,
						 Direction d,
						 const String& vn,
						 const String& bn ) 
{
    if (ds != JAVA_DECL)
    {
	String oper = (d == IN ? " << " : " >> ");
    
	s << bn << oper;
	s << vn << ";\n";
    }
    else
    {
	s << vn << ".unpackFrom(" << bn << ");\n";
    }

    return TRUE;
}

Boolean TypeDescriptor::produceMarshallDefs ( ostream&, DeclStyle, Boolean ) 
{
    return FALSE;
}

Boolean TypeDescriptor::produceStubCode ( ostream&, DeclStyle ) 
{
    return FALSE;
}

/*
 * Generally stub generation is not possible
 */

Boolean TypeDescriptor::stubIsPossible () const
{
    return FALSE;
}

/*
 * Generic output operations
 */

ostream& TypeDescriptor::printAsTypedef ( ostream& s, DeclStyle ) 
{
    return s;
}

/*
 * Print the declaration of an instance of any type. Var_name is the
 * name the instance should have (which may be ignored), elaborated
 * indicates that an elaborated type name is required. Uses prename
 * and postname.
*/
    
ostream& TypeDescriptor::printInstanceOf ( ostream& s,
					   const String& var_name,
					   DeclStyle ds,
					   TypeDescriptor *qualType,
					   Boolean elaborated,
					   Boolean supressConst )
{
    s << preName(ds,elaborated,supressConst);

    if (qualType)
	s << qualType->preName(ds,elaborated,supressConst);
    
    s << " " << massageName(var_name,ds);

    /* Finally postname stuff (eg []) */
    
    if (qualType)
	s << qualType->postName(ds);
    
    s << postName(ds);
    
    return s;
}

/*
 * Protected constructor
 */

TypeDescriptor::TypeDescriptor ( Type kind )
			       : declaredIn(0),
				 isKindOf(kind)
{
#ifdef SG_DEBUG
    debugStream << "TypeDescriptor::TypeDescriptor ("  << (void *)this << ","
		<<  kind << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

TypeDescriptor::~TypeDescriptor ()
{
#ifdef SG_DEBUG
    debugStream << "TypeDescriptor::~TypeDescriptor ()\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

#ifdef NO_INLINES
#  define TDESC_CC_
#  include "TDesc.n"
#  undef TDESC_CC_
#endif
