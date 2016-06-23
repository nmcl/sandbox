/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerT.cc,v 1.1 1997/09/25 15:31:16 nmcl Exp $
 */

/*
 * Stub generator pointer type handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

static const char RCSid[] = "$Id: PointerT.cc,v 1.1 1997/09/25 15:31:16 nmcl Exp $";

/*
 * PUBLIC operations
 */

PointerType::PointerType ( Type type, TypeDescriptor *base, 
			   TypeDescriptor *quals ) 
                         : TypeDescriptor(type),
			   pointerQuals(quals),
			   pointerTo(base),
			   pointerTransfer(STUB_PENDING),
			   deletePointer(kind() == POINTER_TYPE ? TRUE : FALSE)
{
#ifdef SG_DEBUG
    debugStream << "PointerType::PointerType(" << (void *)this << "," 
		<< type << "," << (void *)base << "," << (void *)quals << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

PointerType::~PointerType ()
{
#ifdef SG_DEBUG
    debugStream << "PointerType::~PointerType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean PointerType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "PointerType::addStubModifier("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	if ((sm == pointerTransfer) ||
	    (pointerTransfer == STUB_PENDING))
	{
	    pointerTransfer = sm;
	    return TRUE;
	}
	break;
    case STUB_DELETE:
	deletePointer = TRUE;
	return TRUE;
	break;
    case STUB_NODELETE:
	deletePointer = FALSE;
	return TRUE;
	break;
    default:
	break;
    }

    if (pointerTo)
	return pointerTo->addStubModifier(sm);
    else
	return TypeDescriptor::addStubModifier(sm);
}

Boolean PointerType::attachBaseType ( TypeDescriptor *baseType )
{
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
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

TypeDescriptor *PointerType::deref ()
{
    if (pointerTo)
	return pointerTo;
    else
	return this;
}

/* Print default init for this type (ie = 0) */

String PointerType::defaultInit ( ) const
{
    if (kind() == POINTER_TYPE)
	return String(" = 0");
    else
	return NullString;
}

Boolean PointerType::deleteable () const
{
    return deletePointer;
}
    
Boolean PointerType::freezeType ()
{
    if (pointerTo != 0)
    {
	Boolean result = pointerTo->freezeType();

	if (pointerTransfer == STUB_PENDING)
	{
	    if (pointerTo->isConstant())
		pointerTransfer = STUB_ARG_IN;
	    else
		pointerTransfer = STUB_ARG_INOUT;
	}
	else
	    if ((pointerTo->isConstant()) && (pointerTransfer != STUB_ARG_IN))
	    {
		ostrstream errorStream;

		errorStream << "Stub modifier '" << pointerTransfer << "' "
			    << "not applicable for type: '"
			    << this->typeName(NORMAL_DECL) << "'";
		error(errorStream);
	    }
	
	return result;
    }
    
    else
	return FALSE;
}

Boolean PointerType::isFundamental () const
{
    return FALSE;
}

/*
 * Allow some harmless modifiers here
 */

Boolean PointerType::overrideStubModifiers ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "PointerType::overrideStubModifiers("  << (void *)this
                 << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_AUTOMARSHALL:
    case STUB_NOMARSHALL:
    case STUB_ARG_IN:
    case STUB_ARG_OUT:
    case STUB_ARG_INOUT:
	    return TRUE;
    default:
	return TypeDescriptor::addStubModifier(sm);
    }
    
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
	
	if (ds == IDL_DECL)
	{
	    TypeDescriptor *t = pointerTo;
	    
	    if (kind() == POINTER_TYPE)
	    {
		s += "sequence <";
		
		while (t->kind() == CV_TYPE)
		    t = t->deref();
		
		if (t->kind() == VOID_TYPE)
		    s += "any";
		else
		    s += t->preName(ds, elab, noConst);
	    }
	    else
		s += t->preName(ds, elab, noConst);
	    }
	else
	    s += pointerTo->preName(ds, elab, noConst);
	
	if ((pointedAt == VECTOR_TYPE) || (pointedAt == FUNCTION_TYPE))
	    s += "(";
    }

    if (ds != IDL_DECL)
    {
	if (kind() == POINTER_TYPE)
	    s += "*";
	else
	    s += "&";
    }
    
    if (pointerQuals)
    {
	s += " ";
	s += pointerQuals->preName(ds, elab, noConst);
    } 
     
    if ((ds == IDL_DECL) && (kind() == POINTER_TYPE))
	s += ">";
 
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
    if (pointerTransfer != STUB_PENDING)
	return pointerTransfer;
    
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
 * And by definition pointer to array (which is the same thing)
 */

Boolean PointerType::isTransferable () const
{
    if (pointerTo != 0)
    {
	if (pointerTo->isFundamental())
	    return pointerTo->isTransferable();

	/* Not pointing at fundamental type - may still be ok if I'm a ref */

	if (kind() == POINTER_TYPE)
	{
	    /* Pointer to some type - make sure its not pointer to pointer */
	    TypeDescriptor *t = pointerTo->simplify(CV_TYPE|TYPE_TYPE);
	    Type pKind =  t->kind();

	    if (pKind == POINTER_TYPE)
		return FALSE;
	    else
		return pointerTo->isTransferable();
	}
	else
	    return pointerTo->isTransferable();
    }

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
 * Vector handling code is also here and its AWFUL due to C++ notions
 * of mixing and matching vectors and pointers!!!!
 * We also have to watch out for char& which cannot be sent as char *
 * since the latter is interpretted as a null terminated string!
 */

Boolean PointerType::produceMarshallingCode ( ostream& s,
					      DeclStyle ds,
					      Direction d,
					      const String& varname,
					      const String& buffname )
{
    String newname = varname;
    TypeDescriptor *t = pointerTo->simplify(CV_TYPE|TYPE_TYPE);
    Type pKind = t->kind();

    if ((pKind == VECTOR_TYPE) && (kind() == POINTER_TYPE))
    {
	    newname = String("(*") + newname + ")";
    }
    
    if (d == IN)
    {
	if (pKind == VECTOR_TYPE)
	{
	    s << "if (" << buffname << ".shouldPack(" << varname << "))" << endl;
	}

	/*
	 * Send reference to fundamental types as pointer => maybe take address
	 */
	
	if ((kind() == REFERENCE_TYPE) &&
	    (pointerTo->isFundamental()))
	{
	    if (pKind == CHAR_TYPE)
	    {
		s << "if (" << buffname << ".shouldPack(";
		if (ds == CLIENT_DECL)
		    s << "&";
		s << varname << "))" << endl;
		if (ds == SERVERIMPLEMENTATION_DECL)
		    newname = String("*") + newname;
	    }
	    else
		if (ds == CLIENT_DECL)
		    newname = String("&") + newname;
	}

	return pointerTo->produceMarshallingCode(s, ds, d, newname, buffname);
    }
    else
    {
	if (pKind == VECTOR_TYPE)
	{
	    Boolean result = FALSE;
	    PointerType pt(POINTER_TYPE, t->deref());
	    String name = newname;
	    
	    s << "{\nconst void *p = 0;\nInt32 index = 0;\n";
	    s << "if (" << buffname<< ".unpackPtr(p, index))\n{\n";

	    if ((kind() != REFERENCE_TYPE) || (ds == SERVERIMPLEMENTATION_DECL))
	    {
		s << "if (" << varname << " == 0)\n";

		if (ds == CLIENT_DECL)
		{
		    /* pointers to vectors are handled specially in the client */

		    name = varname + "Ptr";

		    s << "{\n";
		    pt.printInstanceOf(s, name, NORMAL_DECL, 0, FALSE);
    		    s << " = new " << t->typeName(ds) << ";\n";
		    s << varname << " = (" << typeName(NORMAL_DECL) << " )" << name << ";" << endl;
		    s << "}" << endl;
		}
		else
		    s << varname << " = new " << t->typeName(ds) << ";\n";		
	    }
	    result = pointerTo->produceMarshallingCode(s,ds,d,newname,buffname);
	    s << buffname << ".saveUnpackedPtr(" << varname << ",index);\n}\n";
	    
	    if ((kind() != REFERENCE_TYPE) || (ds == SERVERIMPLEMENTATION_DECL))
	    {
		s << "else\n{\nif ((p == 0) && (" << varname << " != 0))\n";
		s << "delete [] " << varname << ";\n";
		if (ds == CLIENT_DECL)
		    s << varname << " = (" << typeName(ds) << ")p;\n}\n";
		else
		    s << varname << " = (" << pt.typeName(ds) << ")p;\n}\n";
	    }
	    s << "}\n";
	    
	    return result;
	}

	/* references to chars are handled carefully! */
	
	if (kind() == REFERENCE_TYPE)
	{
	    if (pKind == CHAR_TYPE)
	    {
		Boolean result = FALSE;
		PointerType pt(POINTER_TYPE, t->deref());
		String name = varname;
	    
		s << "{\nconst void *p = 0;\nInt32 index = 0;\n";
		s << "if (" << buffname<< ".unpackPtr(p, index))\n{\n";

		if (ds == SERVERIMPLEMENTATION_DECL)
		{
		    s << "if (" << varname << " == 0)\n";
		    
		    s << varname << " = new " << t->typeName(ds) << ";\n";
		    newname = String("*") + varname;
		}
		else
		    name = String("&") + varname;
		
		result = pointerTo->produceMarshallingCode(s,ds,d,newname,buffname);
		s << buffname << ".saveUnpackedPtr(" << name << ",index);\n}\n";
		
		if ((kind() != REFERENCE_TYPE) || (ds == SERVERIMPLEMENTATION_DECL))
		{
		    s << "else\n{\nif ((p == 0) && (" << varname << " != 0))\n";
		    s << "delete  " << varname << ";\n";
		    if (ds == CLIENT_DECL)
			s << varname << " = (" << typeName(ds) << ")p;\n}\n";
		    else
			s << varname << " = (" << pt.typeName(ds) << ")p;\n}\n";
		}
		s << "}\n";
		
		return result;

	    }
	    else
		if ((ds == CLIENT_DECL) && (pointerTo->isFundamental()))
		{
		    /*
		     * The server sends back a pointer. In client create a dummy to accept it
		     */
		    PointerType pt(POINTER_TYPE, pointerTo);
		    
		    newname += "Ptr";
		    
		    pt.printInstanceOf(s, newname, NORMAL_DECL, 0, FALSE);
		    s << " = &" << varname << ";\n";
		}
	}
	
	return pointerTo->produceMarshallingCode(s, ds, d, newname, buffname);
    }
}

 
