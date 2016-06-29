/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EnumT.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $
 */

/*
 * Stub generator enumerated type definition handler
 *
 */

#include <strstream.h>
#include <fstream.h>

#ifdef SG_DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef GLOBALINF_H_
#  include "GlobalInf.h"
#endif

#ifndef ENUMT_H_
#  include "EnumT.h"
#endif

#ifndef EXPRESSION_H_
#  include "Expression.h"
#endif

#ifndef DECL_H_
#  include "Decl.h"
#endif

#ifndef DECLLIST_H_
#  include "DeclList.h"
#endif

#ifndef DECLLISTI_H_
#  include "DeclListI.h"
#endif

#ifndef DECLSPEC_H_
#  include "DeclSpec.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef INTERFACE_H_
#  include "Interface.h"
#endif

static const char RCSid[] = "$Id: EnumT.cc,v 1.1 1997/06/09 19:52:22 nmcl Exp $";

long EnumType::enumCount = 0;

/*
 * Constructor
 */

EnumType::EnumType ( const String& name, DeclarationList *enumvals,
		     Boolean complete ) 
                   : TypeDescriptor(ENUMERATION_TYPE),
		     enumFrozen(FALSE),
		     enumIsComplete(complete),
		     enumName(name),
		     enumerators(enumvals),
		     marshallOpts(STUB_PENDING),
		     stubOptionsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "EnumType::EnumType(" << name << ","
		<< (void *)enumvals << "," << complete << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    DeclarationListIterator next(enumerators);
    Declaration *enum_decl;

    if (name == NullString)
    {
	enumName = constructName("_Enum_", enumCount++);
	enumIsAnon = TRUE;
    }
    else
    {
	enumIsAnon = FALSE;
    }

    while ((enum_decl = next()))
    {
	enum_decl->setDtype(this);
    }
}

EnumType::~EnumType ()
{
#ifdef SG_DEBUG
    debugStream << "EnumType::~EnumType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean EnumType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "EnumType::addStubModifier("  << (void *)this
                << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_IGNORE:
	stubOptionsSupressed = TRUE;
	break;
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
        if (marshallOpts == sm)
            return TRUE;
        else
            if (marshallOpts == STUB_PENDING)
            {
                marshallOpts = sm;
                return TRUE;
            }
	break;
    case STUB_REMOTE:
    case STUB_NOREMOTE:
	/* Silently accept these options */
	return TRUE;
	
    default:
	break;
      }
    return TypeDescriptor::addStubModifier(sm);
}

TypeDescriptor *EnumType::combineTypes ( TypeDescriptor * )
{
    error("(stub internal) Attempt to adjust enum type");
    return this;
}

String EnumType::elaborator ( DeclStyle ) const
{
    return "enum ";
}

Boolean EnumType::freezeType ()
{
#ifdef SG_DEBUG
    debugStream << "EnumType::freezeType(" << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    register Declaration *decl;
    DeclarationListIterator next(enumerators);

    if (enumFrozen)
	return TRUE;

    if (marshallOpts == STUB_PENDING)
	marshallOpts = STUB_AUTOMARSHALL;
    
    while ((decl = next()))
    {
	decl->freezeDecl();
    }

    enumFrozen = TRUE;

    return TRUE;
}

/*
 * Allow several modifiers that are harmless
 */

Boolean EnumType::overrideStubModifiers ( StubModifier newSm )
{
#ifdef SG_DEBUG
    debugStream << "EnumType::overrideModifiers("  << (void *)this << ","
		<< newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    switch (newSm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
	return TRUE;
    default:
	return addStubModifier(newSm);
    }
    
}

String  EnumType::postName ( DeclStyle ) const
{
    return NullString;
}

String  EnumType::preName ( DeclStyle, Boolean elaborated, Boolean ) const
{
    String s;

    if (elaborated)
	s += "enum ";

    s += enumName;
    
    return s;
}

String EnumType::signature () const
{
    return enumName;
}

StubModifier EnumType::defaultMarshall () const
{
    return marshallOpts;
}

StubModifier EnumType::defaultTransfer () const
{
    return STUB_ARG_IN;
}

Boolean EnumType::isTransferable () const
{
    return TRUE;
}

Boolean EnumType::produceAllStubs ( ostream& hdr )
{
    printAsTypedef(hdr, NORMAL_DECL);
    
    hdr << ";\n";

    return produceMarshallDefs(hdr, NORMAL_DECL);
}

Boolean EnumType::produceMarshallDefs ( ostream &hdr, DeclStyle ds,
					Boolean full )
{
    if (marshallOpts != STUB_NOMARSHALL)
    {		
        /* generate the code */
	String marshallCodeSuff = GlobalInfo::getMarshallCodeSuff();
	String typePrefix;
	DeclContext *c = getContext();
	ofstream codeStream;

	if (c != 0)
	{
	    typePrefix = c->getContextName(ds);
	}

	if (full)
	{
#ifndef __linux__	  
	    codeStream.open(enumName + marshallCodeSuff);
#else
	    char temp[::strlen(enumName)+::strlen(marshallCodeSuff)+1];
	    ::memset(temp, '\0', ::strlen(enumName)+::strlen(marshallCodeSuff)+1);
	    ::strcpy(temp, enumName);
	    ::strcpy(temp+::strlen(enumName), marshallCodeSuff);
	
	    codeStream.open(temp);
#endif	    
	
	    codeStream << "#include \""
		       << GlobalInfo::getCurrentIf()->getHeaderFname()
		       << "\"\n";
	
	    codeStream << "#include <StubGen/RpcBuffer.h>\n";
	
	    codeStream << "\nRpcBuffer& operator<< ";
	    codeStream << "( RpcBuffer& rpcbuff, " << typePrefix << enumName << "* enumPtr )\n";
	    codeStream << "{\n\treturn rpcbuff.operator<< ((int*)enumPtr);\n";
	    codeStream << "}\n\n";
	
	    codeStream << "RpcBuffer& operator>> ";
	    codeStream << "( RpcBuffer& rpcbuff, " << typePrefix << enumName << "& enumRef )\n";
	    codeStream << "{\n\tint dummy;\n";
	    codeStream << "\trpcbuff >> dummy;\n";
	    codeStream << "\tenumRef = (" << typePrefix << enumName << ")dummy;\n";
	    codeStream << "\treturn rpcbuff;\n";
	    codeStream << "}\n\n"; 
	
	    codeStream << "RpcBuffer& operator>> ";
	    codeStream << "( RpcBuffer& rpcbuff, " << typePrefix << enumName << "*& enumPtr)\n";
	    codeStream << "{\n";
	    codeStream << "\treturn rpcbuff.operator>> ((int*&)enumPtr);\n";
	    codeStream << "}\n\n";

	    codeStream << flush;
	}
	
	/* and the matching extern decls */

	hdr << "\n#ifndef STUB\n";
	hdr << "extern RpcBuffer& operator<< ";
	hdr << "( RpcBuffer&, " << typePrefix << enumName << "* );\n";
	hdr << "extern RpcBuffer& operator>> ";
	hdr << "( RpcBuffer&, " << typePrefix << enumName << "& );\n";
	hdr << "extern RpcBuffer& operator>> ";
	hdr << "( RpcBuffer&, " << typePrefix << enumName << "*&);\n";
	hdr << "#endif" << endl;
    }
    
    return TRUE;
}

Boolean EnumType::stubIsPossible () const
{
    return TRUE;
}

/*
 * Print type description for enumerated types. Prints keyword 'enum'
 * followed by the enum type name, then walks the list of enumerations
 * printing each. Note we don't call printDecl since each enumeration has
 * been given the type of the enum and we don't want that printed
 */
					
ostream& EnumType::printAsTypedef ( ostream& s, DeclStyle ) 
{
    if (!stubOptionsSupressed)
	    s << "/* " << marshallOpts << " */ ";

    s << "\nenum ";
    if (enumName != NullString)
	s << enumName << " ";
    if (enumIsComplete)
    {
	Expression *initVal;

	s << "{";
	if (enumerators != 0)
	{
	    DeclarationListIterator next(enumerators);
	    Declaration *current;
	    
	    current = next();
	    while (current != 0)
	    {
		s << current->getName();
		initVal = current->getInitialiser();
		/* Do not need = here its held as part of the initiliser */
		if (initVal)
		    s << *initVal;

		current = next();
		if (current != 0)
		    s << ", ";
	    }
	}
	s << "}";
    }
    return s;
}
