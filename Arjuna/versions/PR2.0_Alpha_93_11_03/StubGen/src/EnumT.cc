/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: EnumT.cc,v 1.1 1993/11/03 14:47:36 nmcl Exp $
 */

/*
 * Stub generator enumerated type definition handler
 *
 */

#include <strstream.h>
#include <fstream.h>

#ifdef DEBUG
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

static const char RCSid[] = "$Id: EnumT.cc,v 1.1 1993/11/03 14:47:36 nmcl Exp $";

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
		     marshallOpts(STUB_PENDING)
{
#ifdef DEBUG
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

    while (enum_decl = next())
    {
	enum_decl->setDtype(this);
    }
}

EnumType::~EnumType ()
{
#ifdef DEBUG
    debugStream << "EnumType::~EnumType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean EnumType::addStubModifier ( StubModifier sm )
{
#ifdef DEBUG
    debugStream << "EnumType::addStubModifier("  << (void *)this
                << "," << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
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
    default:
	return TypeDescriptor::addStubModifier(sm);
    }
}

TypeDescriptor *EnumType::combineTypes ( TypeDescriptor * )
{
    error("(stub internal) Attempt to adjust enum type");
    return this;
}

Boolean EnumType::freezeType ()
{
#ifdef DEBUG
    debugStream << "EnumType::freezeType(" << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    register Declaration *decl;
    DeclarationListIterator next(enumerators);

    if (enumFrozen)
	return TRUE;

    if (marshallOpts == STUB_PENDING)
	marshallOpts = STUB_AUTOMARSHALL;
    
    while (decl = next())
    {
	decl->freezeDecl();
    }

    enumFrozen = TRUE;

    return TRUE;
}

String  EnumType::postName ( DeclStyle ) const
{
    return NullString;
}

String  EnumType::preName ( DeclStyle, Boolean, Boolean ) const
{
    return enumName;
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
    printAsTypedef(hdr, CLIENT_DECL);
    
    hdr << ";\n";
    
    if (marshallOpts != STUB_NOMARSHALL)
    {		
        /* generate the code */
	String marshallCodeSuff = GlobalInfo::getMarshallCodeSuff();
	ofstream codeStream;
	
#ifndef __linux__	  
	codeStream.open(enumName + marshallCodeSuff);
#else
	char temp[::strlen(enumName)+::strlen(marshallCodeSuff)+1];
	::memset(temp, '\0', ::strlen(enumName)+::strlen(marshallCodeSuff)+1);
	::strcpy(temp, enumName);
	::strcpy(temp+::strlen(enumName), marshallCodeSuff);
	
	cout << "**Creating file " << temp << "**" << endl;
	
	codeStream.open(temp);
#endif	    
	
	codeStream << "#include \""
		   << GlobalInfo::getCurrentIf()->getHeaderFname()
		   << "\"\n";
	
	codeStream << "#include <StubGen/RpcBuffer.h>\n";
	
	codeStream << "\nRpcBuffer& operator<< ";
	codeStream << "( RpcBuffer& rpcbuff, " << enumName << "* enumPtr )\n";
	codeStream << "{\n\treturn rpcbuff.operator<< ((int*)enumPtr);\n";
	codeStream << "}\n\n";
	
	codeStream << "RpcBuffer& operator>> ";
	codeStream << "( RpcBuffer& rpcbuff, " << enumName << "& enumRef )\n";
	codeStream << "{\n\tint dummy;\n";
	codeStream << "\trpcbuff >> dummy;\n";
	codeStream << "\tenumRef = (" << enumName << ")dummy;\n";
	codeStream << "\treturn rpcbuff;\n";
	codeStream << "}\n\n"; 
	
	codeStream << "RpcBuffer& operator>> ";
	codeStream << "( RpcBuffer& rpcbuff, " << enumName << "*& enumPtr)\n";
	codeStream << "{\n";
	codeStream << "\treturn rpcbuff.operator>> ((int*&)enumPtr);\n";
	codeStream << "}\n\n";

	codeStream << flush;

	/* and the matching extern decls */

	hdr << "\nextern RpcBuffer& operator<< ";
	hdr << "( RpcBuffer&, " << enumName << "* );\n";
	hdr << "extern RpcBuffer& operator>> ";
	hdr << "( RpcBuffer&, " << enumName << "& );\n";
	hdr << "extern RpcBuffer& operator>> ";
	hdr << "( RpcBuffer&, " << enumName << "*&);\n\n";
	hdr << flush;
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
