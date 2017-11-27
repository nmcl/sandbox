/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassT.cc,v 1.29.6.1 1996/10/10 12:26:48 ngdp Exp $
 */

/*
 * Stub generator class definition handler
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

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

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

#ifndef CLASST_H_
#  include "ClassT.h"
#endif

#ifndef FUNCT_H_
#  include "FuncT.h"
#endif

#ifndef VISSPEC_H_
#  include "VisSpec.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
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

#ifndef INTERFACE_H_
#  include "Interface.h"
#endif

extern ClassType *currentclass;

static const char RCSid[] = "$Id: ClassT.cc,v 1.29.6.1 1996/10/10 12:26:48 ngdp Exp $";

#ifndef MAINNAME
#  define MAINNAME "main"
#endif

static const String implpref("Local");
static const String remotepref("Remote");
static const String serverpref("Server");

/*
 * PUBLIC operations.
 */

/*
 * Constructor
 */

ClassType::ClassType ( const String& name, ClassSpecifier ckind,
		       BaseClass *bases, DeclContext *parent,
		       Boolean nameless, Boolean willBeDefn )
                     : TypeDescriptor(AGGREGATE_TYPE),
		       classCtors(0),
		       classDtor(0),
		       baseList(0),
		       deriveList(bases),
		       vBaseList(0),
		       classClientName(remotepref + name),
		       classComplete(FALSE),
		       classFullDecl(0),
		       classHashval(hashIt(name)),
		       classImplName(implpref + name),
		       classIsRemote(TRUE),
		       classKind(ckind),
		       classMarshallOpts(STUB_PENDING),
		       classMembers(new ClassDeclContext(this,parent,bases)),
		       className(name),
		       classIsNameless(nameless),
		       classServerName(serverpref + name),
		       classStubLoc(STUB_PENDING),
		       doneMarshallDefs(0),
		       doneTypeDefs(0),
		       exceptionsThrown(FALSE),
		       frozen(FALSE),
		       hasConstructor(FALSE),
		       hasDestructor(FALSE),
		       hasVirtuals(FALSE),
		       stubOptionsSupressed(FALSE),
		       warningsSupressed(FALSE)
{
#ifdef SG_DEBUG
    debugStream << "ClassType::ClassType(" << (void *)this << "," << name
		<< "," << (void *)bases << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (ckind == CLASS_TYPE)
    {
	defVisibility = CLASS_PRIVATE;
	curVisibility = CLASS_PRIVATE;
    }
    else
    {
	defVisibility = CLASS_PUBLIC;
	curVisibility = CLASS_PUBLIC;
    }

    /* Calculate ALL virtual bases of this class */

    if (willBeDefn)
	findVbases ();
}

ClassType::~ClassType ()
{
}

void ClassType::addCtor ( Declaration *ctDecl )
{
    if (classCtors != 0)
	classCtors->appendDecl(ctDecl);
    else
	classCtors = new DeclarationList(ctDecl);
}

void ClassType::addDtor ( Declaration *dtDecl )
{
    if (classDtor != 0)
    {
	ostrstream errorStream;

	errorStream << "class " << className
		    << " already has a destructor\n";
	error(errorStream);
    } else
	classDtor = dtDecl;
}

/*
 * Check if name is name of current class or ANY from which it is derived
 * Return pointer to appropriate class if so
 */

ClassType *ClassType::nameIsClassName ( const String& name ) 
{
    BaseClass *bases = deriveList;
    ClassType *rval = 0;
    ClassType *nclass = 0;

    /* Is it me? */

    if (name == className)
	return this;

    /* check my bases */

    while ((rval == 0) && (bases != 0))
    {
	nclass = bases->getClass();
	bases = bases->getNext();
	rval = nclass->nameIsClassName(name);
    }

    return rval;
}

void ClassType::changeVisibility ( VisibilitySpec newvis )
{
    if (classFullDecl)
    {
	classFullDecl->changeVisibility(newvis);
    }
    else
    {
	if (classKind == CLASS_TYPE || classKind == STRUCT_TYPE)
	    curVisibility = newvis;
	else
	    error("cannot change visibility of union members");
    }
}

Boolean ClassType::checkClassProps ( StubModifier tocheck )
{
    if (classFullDecl)
	return classFullDecl->checkClassProps(tocheck);
    else
    {
	BaseClass *bc = deriveList;
	Boolean propsOk = TRUE;

	/* Compare requested stub properties of parent to me */

	switch (tocheck)
	{
	case STUB_REMOTE:
	    if (classStubLoc != tocheck)
		    return FALSE;
	    break;
	case STUB_NOREMOTE:
	case STUB_NOCLIENT:
	    if (classStubLoc == STUB_REMOTE)
		    return FALSE;
	    break;
	case STUB_NOMARSHALL:
	    break;
	case STUB_AUTOMARSHALL:
	case STUB_USERMARSHALL:
	    if (classMarshallOpts == STUB_NOMARSHALL)
		return FALSE;
	    break;
	default:
	    return FALSE;
	}
	
	/* ok so far - check my bases */
	while (bc != 0 && propsOk)
	{
	    if (!(propsOk = bc->getClass()->checkClassProps(tocheck)))
	    {
		ostrstream errorStream;

		Boolean warningsOn = errorLog.warningsEnabled();
		
		if (warningsSupressed && warningsOn)
		{
		    errorLog.supressWarnings(FALSE);
		}
		
		errorStream << "stub property '" << tocheck
			    << "' of derived class '" << className
			    << "' may not be compatible with that specified for"
			    << " base class '"
			    << bc->getClass()->typeName(NORMAL_DECL) << "'";

		propsOk = TRUE;
		caution(errorStream);
		
		if (warningsSupressed && warningsOn)
		{
		    errorLog.enableWarnings(FALSE);
		}
		
	    }
	    bc = bc->getNext();
	}

	return propsOk;
    }
}

/*
 * Add stub modifier to class definition. These are only those that 
 * were explicitly stated. Rely on freezeType to set up defaults
 * later if need be.
 */

Boolean ClassType::addStubModifier ( StubModifier sm )
{
#ifdef SG_DEBUG
    debugStream << "ClassType::addStubModifier(" << (void *)this
		<< ", " << sm <<  ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    Boolean smOk = TRUE;
    Boolean conflicting = FALSE;
    StubModifier earlier = STUB_PENDING;
    
    switch (sm)
    {
    case STUB_REMOTE:
	earlier = classStubLoc;
	if (classStubLoc == sm)
	    break;
	else
	    if (classStubLoc == STUB_PENDING)
	    {
		if (!classIsNameless)
		{
		    classStubLoc = STUB_REMOTE;
		    smOk = checkClassProps(classStubLoc);
		}
		break;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_NOREMOTE:
	earlier = classStubLoc;
	if (classStubLoc == sm)
	    break;
	else
	    if (classStubLoc == STUB_PENDING)
	    {
		classStubLoc = STUB_NOREMOTE;
		smOk = checkClassProps(classStubLoc);
		break;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_AUTOMARSHALL:
    case STUB_USERMARSHALL:
    case STUB_NOMARSHALL:
	earlier = classMarshallOpts;
	if (classMarshallOpts == sm)
	    break;
	else
	    if (classMarshallOpts == STUB_PENDING)
	    {
		classMarshallOpts = sm;
		smOk = checkClassProps(classMarshallOpts);
		break;
	    }
	    else
		conflicting = TRUE;
	break;
    case STUB_NOWARNINGS:
	warningsSupressed = TRUE;
	break;
    case STUB_IGNORE:
	stubOptionsSupressed = TRUE;
	break;
     default:
	break;
    }
    
    if (!smOk)
    {
	ostrstream errorStream;

	errorStream << "Stub modifier '" << sm << "' ";
	if (conflicting)
	    errorStream << "conflicts with earlier modifier '" << earlier << "'";
	else
	    errorStream << " illegal in this declaration";

	error(errorStream);
    }
    return smOk;
    
}

TypeDescriptor *ClassType::combineTypes ( TypeDescriptor *newtype )
{
#ifdef SG_DEBUG
    debugStream << "ClassType::combineTypes(" << (void *)this
		<< ", " << (void *)newtype << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (classFullDecl)
	return classFullDecl->combineTypes(newtype);
    else
	if (classComplete)
	{
	    ostrstream errorStream;

	    /*
	     * Attempt to handle typedef struct x {} x;
	     */

	    if (newtype->kind() == kind())
		if (newtype->typeName(NORMAL_DECL) == typeName(NORMAL_DECL))
		    return this;

	    errorStream << "attempt to redefine " 
			<< newtype->typeName(NORMAL_DECL) << " as ";
	    printInstanceOf(errorStream, NullString, NORMAL_DECL, 0, TRUE);
	    error(errorStream);
	}
	else
	{
	    classFullDecl = (ClassType *)newtype;
	    if (classStubLoc != STUB_PENDING)
		newtype->addStubModifier(classStubLoc);
	    if (classMarshallOpts != STUB_PENDING)
		newtype->addStubModifier(classMarshallOpts);
	}

    return this;
}

String ClassType::elaborator ( DeclStyle ds ) const
{
    String name;

    switch (classKind)
    {
    case CLASS_TYPE:
	if (ds != IDL_DECL)
	    name = "class ";
	else
	    name = "interface ";
	break;
    case STRUCT_TYPE:
	name = "struct ";
	break;
    case UNION_TYPE:
	name = "union ";
	break;
    }

    return name;
}

/*
 * Freeze declaration of a class type. This includes setting stub
 * generation defaults (which may be ignored if they have already been
 * set explicitly). Note that depending upon whether this object represents
 * a forward reference or a full class the defaults are different.
 */

Boolean ClassType::freezeType ()
{
#ifdef SG_DEBUG
    debugStream << "ClassType::freezeType(" << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (frozen)
	return TRUE;

    frozen = TRUE;

    if (classFullDecl)
    {
	if (warningsSupressed)
	    classFullDecl->addStubModifier(STUB_NOMARSHALL);

	if (classStubLoc != STUB_PENDING)
	    classFullDecl->addStubModifier(classStubLoc);
	
	if (classMarshallOpts != STUB_PENDING)
	    classFullDecl->addStubModifier(classMarshallOpts);
	
	classFullDecl->freezeType();

	classStubLoc = defaultLocation();
	classMarshallOpts = defaultMarshall();
	classClientName = classFullDecl->simpleTypeName(CLIENT_DECL);
	classImplName = classFullDecl->simpleTypeName(SERVERIMPLEMENTATION_DECL);
	classServerName = classFullDecl->simpleTypeName(SERVER_DECL);
    }
    else
    {
	/* freeze base classes (if any) */

	if (classComplete && (deriveList != 0))
	{
	    ostrstream errorStream;

	    deriveList->freeze();
	    
	    /* Pick up default attributes from parent class(es) if needed */

	    if (classStubLoc == STUB_PENDING)
	    {
		classStubLoc = deriveList->defaultLocation();
		errorStream << "Inheriting stub option : " << classStubLoc;
		warning(errorStream);
	    }

	    if (classMarshallOpts == STUB_PENDING)
	    {
		classMarshallOpts = deriveList->defaultMarshall();
		errorStream << "Inheriting stub option : " << classMarshallOpts;
		warning(errorStream);
	    }
	}
	
	/* Fill in default stub attributes if required */

	if (classIsNameless)
	{
	    classStubLoc = STUB_NOREMOTE;
	    classMarshallOpts = STUB_NOMARSHALL;
	}

	if (classStubLoc == STUB_PENDING)
	{
	    switch (classMarshallOpts)
	    {
	    case STUB_AUTOMARSHALL:
		addStubModifier(STUB_NOREMOTE);
		break;
	    default:
		if (classComplete)
		    addStubModifier(STUB_REMOTE);
		else
		    addStubModifier(STUB_NOREMOTE);
		}
	}

	if (classMarshallOpts == STUB_PENDING)
	{
	    switch (classStubLoc)
	    {
	    case STUB_REMOTE:
		addStubModifier(STUB_NOMARSHALL);
		break;
	    default:
		addStubModifier(STUB_AUTOMARSHALL);
	    }
	}

	if (classStubLoc == STUB_REMOTE)
	{
	    classClientName = remotepref + className;
	    classImplName = implpref + className;
	    classServerName = serverpref + className;
	} else
	{
	    classClientName = className;
	    classImplName = className;
	    classServerName = className;
	}

	/* Freeze member list too */

	Declaration *decl;
	int i = 0;

	while ((decl = classMembers->getOneMember(i++)))
	{
	    decl->freezeDecl();
	}
	
    }    

    return TRUE;
}

Boolean ClassType::makeComplete ()
{
    if (classFullDecl)
	return classFullDecl->makeComplete();
    else
	classComplete = TRUE;
    return TRUE;
}

String ClassType::postName ( DeclStyle ) const 
{
    return NullString;
}

String ClassType::preName ( DeclStyle ds, Boolean elab, Boolean ) const
{
    String name;
    
    if (elab)
	name = elaborator(ds);

    name += simpleTypeName(ds);

    return name;
}

String ClassType::signature () const
{
    return className;
}

String ClassType::simpleTypeName ( DeclStyle ds ) const
{  
    if (classFullDecl)
	return classFullDecl->simpleTypeName(ds);
    else  
	switch (ds)
	{
	case CLIENT_DECL:
	    return classClientName;    
	case SERVER_DECL:
	    return classServerName; 
	case SERVERIMPLEMENTATION_DECL:
	    return classImplName;
	case NORMAL_DECL:
	default:
	    return className;
	}
}

String ClassType::typeName ( DeclStyle ds, Boolean elab, Boolean qualify ) const
{  
    if (classFullDecl)
	return classFullDecl->typeName(ds, elab, qualify);
    else
    {
	String name;
	
	if (elab)
	    name = elaborator(ds);

	if (qualify && classMembers)
	{
	    DeclContext *outer = classMembers->getEnclosingContext();

	    if (outer)
		name += outer->getContextName(ds);
	}
	
	name += simpleTypeName(ds);

	return name;
    }
}

StubModifier ClassType::defaultLocation () const
{
    if (classFullDecl)
	return classFullDecl->defaultLocation();
    else
	return classStubLoc;
    
}

StubModifier ClassType::defaultMarshall () const
{
    if (classFullDecl)
	return classFullDecl->defaultMarshall();
    else
	return classMarshallOpts;
}

StubModifier ClassType::defaultTransfer () const
{
    if (isTransferable())
	return STUB_ARG_IN;
    else
	return TypeDescriptor::defaultTransfer();
}

Boolean ClassType::isTransferable () const
{
    if (classFullDecl)
	return classFullDecl->isTransferable();
    else
	return (((classMarshallOpts == STUB_AUTOMARSHALL) ||
		 (classMarshallOpts == STUB_USERMARSHALL)) ? TRUE : FALSE);
}

Boolean ClassType::produceAllStubs ( ostream& masterHdr )
{
#ifdef SG_DEBUG
    debugStream << "ClassType::produceAllStubs(" << (void *)this
		<< ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (classFullDecl)
	return classFullDecl->produceAllStubs(masterHdr);
    else if (classStubLoc == STUB_REMOTE)
    {
	ofstream classHeader;
	ofstream clientCode;
	ofstream serverCode;
	ofstream serverMain;
	ofstream shadowCode;
	
	String classHdrSuff = GlobalInfo::getClassHdrSuff();
	String clientCodeSuff = GlobalInfo::getClientCodeSuff();
	String serverCodeSuff = GlobalInfo::getServerCodeSuff();
	String servermCodeSuff = GlobalInfo::getServermCodeSuff();
	String shadowCodeSuff = GlobalInfo::getShadowCodeSuff();

	Boolean doShadows = (GlobalInfo::getOutputStyle() == SHADOWS_STYLE) ? TRUE : FALSE;
	
	
#ifdef __linux__
	// g++ on linux does not open ofstreams correctly
	char* temp = ::new char[::strlen(className) +
				::strlen(classHdrSuff) +1];
	::memset(temp, '\0', ::strlen(className) + ::strlen(classHdrSuff) +1);
	::strcpy(temp, className);
	::strcpy(temp+::strlen(className), classHdrSuff);

	classHeader.open(temp);
	::delete [] temp;

	char* temp1 = ::new char[::strlen(className)+::strlen(clientCodeSuff)+1];
	::memset(temp1, '\0', ::strlen(className)+::strlen(clientCodeSuff)+1);
	::strcpy(temp1, className);
	::strcpy(temp1+::strlen(className), clientCodeSuff);

	clientCode.open(temp1);
	::delete [] temp1;
	
	char* temp2 = ::new char[::strlen(className)+::strlen(serverCodeSuff)+1];
	::memset(temp2, '\0', ::strlen(className)+::strlen(serverCodeSuff)+1);
	::strcpy(temp2, className);
	::strcpy(temp2+::strlen(className), serverCodeSuff);
	
	serverCode.open(temp2);
	::delete [] temp2;
	
	char* temp3 = ::new char[::strlen(className)+::strlen(servermCodeSuff)+1];
	::memset(temp3, '\0', ::strlen(className)+::strlen(servermCodeSuff)+1);
	::strcpy(temp3, className);
	::strcpy(temp3+::strlen(className), servermCodeSuff);
	
	serverMain.open(temp3);
	::delete [] temp3;

	if (doShadows)
	{
	    char* temp4 = ::new char[::strlen(className)+::strlen(shadowCodeSuff)+1];
	    ::memset(temp4, '\0', ::strlen(className)+::strlen(shadowCodeSuff)+1);
	    ::strcpy(temp4, className);
	    ::strcpy(temp4+::strlen(className), shadowCodeSuff);
	
	    shadowCode.open(temp4);
	    ::delete [] temp4;
	}
	
#else
	classHeader.open(className + classHdrSuff);
	clientCode.open(className + clientCodeSuff);
	serverCode.open(className + serverCodeSuff);
	serverMain.open(className + servermCodeSuff);

	if (doShadows)
	    shadowCode.open(className + shadowCodeSuff);
#endif
	
	masterHdr << "#include \"" << className + classHdrSuff;
	masterHdr << "\"\n";

	/* Class descriptions */
	
	GlobalInfo::stamp(classHeader);
	
	/* First print list of standard includes */

	classHeader << "#ifndef STUB\n";	
	classHeader << "#include <StubGen/RpcBuffer.h>\n";
	classHeader << "#include <StubGen/ClientRpcMan.h>\n\n";
	
	printAsTypedef(classHeader, CLIENT_DECL);
	classHeader << ";\n\n";

	printAsTypedef(classHeader, SERVER_DECL);
	classHeader << ";\n#endif\n\n";

	printAsTypedef(classHeader, SERVERIMPLEMENTATION_DECL);
	classHeader << ";\n\n";

	if (doShadows)
	{
	    printAsTypedef(classHeader, SHADOWS_DECL);
	    classHeader << ";\n\n";
	}

	classHeader << flush;
	
	/* Code Generation */

	if (doShadows)
	{
	    GlobalInfo::stamp(shadowCode);
	    produceStubCode(shadowCode, SHADOWS_DECL);
	}
	
	GlobalInfo::stamp(clientCode);
	produceStubCode(clientCode, CLIENT_DECL);
	GlobalInfo::stamp(serverCode);
	produceStubCode(serverCode, SERVER_DECL);
	
	/* Simple main program for server - may not be needed */
	
	GlobalInfo::stamp(serverMain);
	serverMain << "#include \""
		   << GlobalInfo::getCurrentIf()->getHeaderFname() 
		   << "\"\n";
	
	serverMain << "int " << MAINNAME 
		   << " ( int argc, char **argv )\n{\n";
	
	serverMain << typeName(SERVER_DECL) << " one";
	serverMain << className << "Server;\n\n";
	serverMain << "one" << className << "Server.serve(argc, argv);\n";
	serverMain << "return 0;\n}\n";
	
	clientCode << flush;
	serverCode << flush;
	serverMain << flush;

	if(doShadows)
	    shadowCode << flush;
    }
    else
    {
	printAsTypedef(masterHdr, SERVERIMPLEMENTATION_DECL);
	masterHdr << ";\n";
    }
	
    produceMarshallDefs(masterHdr, NORMAL_DECL);
        
    masterHdr << flush;

    return TRUE;
}

Boolean ClassType::produceMarshallDefs ( ostream& s, DeclStyle ds,
					 Boolean full )
{
    if (classFullDecl)
	classFullDecl->produceMarshallDefs(s,ds,full);
    else
    {
	Declaration *decl = 0;
	int i = 0;
	
	
	while ((decl = classMembers->getOneMember(i++)))
	{
	    /* Check if this decl is a typedef and requires marshalling */
	    
	    StubModifier sm = decl->getMarshall(FALSE);
	    
	    if (decl->isTypedef() &&
		((sm == STUB_AUTOMARSHALL) || (sm == STUB_USERMARSHALL)))
	    {
		decl->produceMarshallDefs(s, NORMAL_DECL, full);
	    }
	}

	if ((classMarshallOpts != STUB_NOMARSHALL) &&
	    !doneMarshallDefs)
	{

	    doneMarshallDefs = TRUE;
	    
	    s << "\n#ifndef STUB\n";
	    
	    /* Generate inline operator defs */

	    s << "\n";
	    printTemplatePrefix(s);
	    s << "extern RpcBuffer& operator<< ";
	    s << "( RpcBuffer&, const " << typeName(SERVERIMPLEMENTATION_DECL);
	    s << " *);\n";
	    
	    printTemplatePrefix(s);
	    s << "extern RpcBuffer& operator>> ";
	    s << "( RpcBuffer&, " << typeName(SERVERIMPLEMENTATION_DECL) << " *&);\n";

	    printTemplatePrefix(s);
	    s << "\nextern RpcBuffer& operator<< ";
	    s << "( RpcBuffer& rpcbuff, const " << typeName(SERVERIMPLEMENTATION_DECL) << "& topack);\n";

	    printTemplatePrefix(s);
	    s << "extern RpcBuffer& operator>> ";
	    s << "( RpcBuffer& rpcbuff, " << typeName(SERVERIMPLEMENTATION_DECL) << "& tounpack);\n";
	    s << "#endif\n\n";
	    
	    if (full && classComplete)
	    {
		/* generate the code */
		ofstream codeStream;
		String marshallCodeSuff = GlobalInfo::getMarshallCodeSuff();

#ifndef __linux__	  
		codeStream.open(className + marshallCodeSuff);
#else
		char temp[::strlen(className)+::strlen(marshallCodeSuff)+1];
		::memset(temp, '\0', ::strlen(className)+::strlen(marshallCodeSuff)+1);
		::strcpy(temp, className);
		::strcpy(temp+::strlen(className), marshallCodeSuff);

		codeStream.open(temp);
#endif	    
		GlobalInfo::stamp(codeStream);

		codeStream << "#include \""
			   << GlobalInfo::getCurrentIf()->getHeaderFname() 
			   << "\"\n\n";

		codeStream << "#include <StubGen/RpcBuffer.h>\n";

		printTemplatePrefix(codeStream);
		codeStream << "\nRpcBuffer& operator<< ";
		codeStream << "( RpcBuffer& rpcbuff, const " << typeName(SERVERIMPLEMENTATION_DECL) << "& topack)\n";
		codeStream << "{\n";
		codeStream << "\trpcbuff << " << classHashval << ";\n";
		codeStream << "\ttopack.marshall(rpcbuff);\n\treturn rpcbuff;\n";
		codeStream << "}\n\n";
		
		printTemplatePrefix(codeStream);
		codeStream << "\nRpcBuffer& operator>> ";
		codeStream << "( RpcBuffer& rpcbuff, " << typeName(SERVERIMPLEMENTATION_DECL) << "& tounpack)\n";
		codeStream << "{\n";
		codeStream << "\tInt32 hashVal;\n";
		codeStream << "\trpcbuff >> hashVal;\n";
		codeStream << "\tif (hashVal != " << classHashval << ")\n";
		codeStream << "\t\trpcbuff.setState(BUFFER_BAD);\n\telse\n";
		codeStream << "\t\ttounpack.unmarshall(rpcbuff);\n\treturn rpcbuff;\n";
		codeStream << "}\n\n";

		printTemplatePrefix(codeStream);	    
		codeStream << "\nRpcBuffer& operator<< ";
		codeStream << "( RpcBuffer& rpcbuff, const " << typeName(ds) << " *classptr )\n";
		codeStream << "{\n";
		codeStream << "if (rpcbuff.shouldPack(classptr))\n";
		codeStream << "rpcbuff << *classptr;\n";
		codeStream << "return rpcbuff;\n";
		codeStream << "}\n\n";

		printTemplatePrefix(codeStream);	    
		codeStream << "\nRpcBuffer& operator>> ";
		codeStream << "( RpcBuffer& rpcbuff, " << typeName(ds) << " *& classptr )\n";
		codeStream << "{\nconst void *p = 0;\nInt32 index = 0;\n";
		codeStream << "if (rpcbuff.unpackPtr(p, index))\n{\n";
		codeStream << "if (classptr == 0)\n";
		codeStream << "classptr = new " << typeName(ds) << "();\n";
		codeStream << "rpcbuff >> *classptr;\n";
		codeStream << "rpcbuff.saveUnpackedPtr(classptr,index);\n}\n";
		codeStream << "else\n{\n";
		codeStream << "if (p == 0)\ndelete classptr;\n";
		codeStream << "classptr = (" << typeName(ds) << " *)p;\n}";
		codeStream << "return rpcbuff;\n";
		codeStream << "}\n\n";
		
		if (classMarshallOpts == STUB_AUTOMARSHALL)
		{
		    if (ds != IDL_DECL)
		    {
			produceClassMarshallingCode(codeStream, ds, IN);
			produceClassMarshallingCode(codeStream, ds, OUT);
		    }
		}
		codeStream << flush;
	    }
	}
	
	s << flush;
    }
    return TRUE;
}

Boolean ClassType::produceStubCode ( ostream& s, DeclStyle ds )
{
    switch (ds)
    {
    case CLIENT_DECL:
	produceClientCode(s);
	break;
    case SERVER_DECL:
	produceServerCode(s);
	break;
    case SHADOWS_DECL:
	produceShadowsCode(s);
    default:
	break;
    }
    return TRUE;
}

Boolean ClassType::stubIsPossible () const
{
    if (classComplete)
    {
	if ((classStubLoc == STUB_REMOTE) ||
	    (classMarshallOpts != STUB_NOMARSHALL))
	    return TRUE;
    }
    return FALSE;
}

/*
 * Class output routines.
 */

ostream& ClassType::printAsTypedef ( ostream& s, DeclStyle ds )
{
    int i = 0;
    Declaration *decl;
    VisibilitySpec currentVis, lastVis;
	
    lastVis = defVisibility;

    switch (ds)
    {
    case SHADOWS_DECL:
	printShadowsTypedef(s);
	break;
    case CLIENT_DECL:
	printClientTypedef(s);
	break;
    case SERVER_DECL:
	printServerTypedef(s);
	break;
    case IDL_DECL:
	printIdlTypedef(s);
	break;
	
    /* BEWARE DELIBERATE FALL THROUGH IN FOLLOWING CASES */
	
    case SERVERIMPLEMENTATION_DECL:
	if (classImplName != className)
	{   
	    s << "\n#ifdef STUB\n"
	      << "#  define " << classImplName << " " << className << "\n"
	      << "#endif\n\n";
	}
    case NORMAL_DECL:
	if (!stubOptionsSupressed)
	    s << "/* " << classStubLoc << ", " << classMarshallOpts << " */ ";
	printClassHeader(s, ds);
	
	if (classComplete && ((doneTypeDefs & ds) == 0))
	{
	    doneTypeDefs |= ds;
	    
	    s << "\n{\n";
	    
	    while ((decl = classMembers->getOneMember(i++)) != 0)
	    {
		if ((currentVis = decl->getVisibility()) != lastVis)
		{
		    lastVis = currentVis;
		    switch (currentVis)
		    {
		    case CLASS_PRIVATE:
			s << "private:\n";
			break;
		    case CLASS_PROTECTED:
			s << "protected:\n";
			break;
		    case CLASS_PUBLIC:
			s << "public:\n";
			break;
		    default:
			break;
		    }
		}
		s << "\t";
		decl->printDecl(s,ds);
	    }
	    
	    if (classMarshallOpts == STUB_AUTOMARSHALL)
	    {
		s << "#ifndef STUB\n";
		
		if (currentVis != CLASS_PUBLIC)
		    s << "public:\n";
		
		if (hasVirtuals)
		{
		    s << "\tvirtual void marshall (RpcBuffer&) const;\n";
		    s << "\tvirtual void unmarshall (RpcBuffer&);\n";
		}
		else
		{
		    s << "\tvoid marshall (RpcBuffer&) const;\n";
		    s << "\tvoid unmarshall (RpcBuffer&);\n";
		}
		s << "\n";
		s << "#endif\n";
	    }
	    
	    s << "}";
	}
    }

    return s;
}

ostream& ClassType::printClassHeader ( ostream& s, DeclStyle ds, Boolean full )
{
    BaseClass *bc = deriveList;

    printTemplatePrefix(s);

    /* Beware of deliberate supression of virtual mechansim here */

    s << ClassType::preName(ds, TRUE);
	
    if (full)
    {
	/* print out full derivation info */
	if (bc != 0)
	    s << ": ";
	    
	while (bc != 0)
	{
	    bc->printBaseClass(s, ds);
	    bc = bc->getNext();
	    if (bc)
		s << ", ";
	}
    }

    return s;
}

Boolean ClassType::produceClassMarshallingCode ( ostream& s, DeclStyle ds,
						 Direction d )
{
    int i = 0;
    Declaration *decl;
    String fname = (d == IN ? "marshall" : "unmarshall");

    printTemplatePrefix(s);
    
    s << "\nvoid " << typeName(ds) << "::";
    s << fname;

    s << " ( RpcBuffer& rpcBuff )";

    if (d == IN)
	s << " const";
    
    s << "\n{\n";

    /* Generate calls on parent marshall routines */
    {
	/* virtual bases first */

	BaseClass *blist = vBaseList;

	while (blist)
	{
	    ClassType *cl = blist->getClass();

	    s << cl->typeName(ds) << "::";
	    s << fname << "(rpcBuff);\n";

	    blist = blist->getNext();
	}

	/* now non-virtual */

	blist = baseList;
	
	while (blist)
	{
	    ClassType *cl = blist->getClass();

	    s << cl->typeName(ds) << "::";
	    s << fname << "(rpcBuff);\n";

	    blist = blist->getNext();
	}
    }

    while ((decl = classMembers->getOneMember(i++)))
    {
	/* Get both default & explicit marshall opts */
	StubModifier expMarshallOpts = decl->getMarshall(TRUE);
	StubModifier defMarshallOpts = decl->getMarshall(FALSE);

	/* If any say marshall - do it */
	
	if ((defMarshallOpts == STUB_AUTOMARSHALL) ||
	    (defMarshallOpts == STUB_USERMARSHALL))
	    decl->marshall(s, d, ds, "rpcBuff");
	else
	{
	    /* Give an error unless its an EXPLICIT no marshall */
	    
	    if (expMarshallOpts != STUB_NOMARSHALL)
	    {
		ostrstream errorStream;

		decl->printDecl(errorStream, NORMAL_DECL, "");
		errorStream << " cannot be " << fname << "ed";
		error(errorStream);
	    }
	}
	
    }
    s << "\n}\n\n";
    return TRUE;
}

ostream& ClassType::printTemplatePrefix ( ostream& s )
{
    return s;
}

/*
 * Set up base class lists
 * deriveList is the exact list of classes we are derived from
 * baseList is only those that are not virtual
 * vBaseList is ALL virtual bases both direct AND indirect
 * Also sets up if this class (or its parents) have virtual functions
 */

void ClassType::findVbases ()
{
    BaseClass *blist = deriveList;
    BaseClass vBases(0);		/* DUMMY - discarded later */
    BaseClass bases(0);			/* DUMMY - discarded later */

    if (blist)
    {
	/* Walk down the specified base class list for this class */

	while (blist)
	{
	    ClassType *bclass = blist->getClass();
	    BaseClass *dblist = bclass->getVbaseList();

	    if (bclass->hasVirtualFuncs())
		hasVirtuals = TRUE;
	    
	    /* If its virtual add it into the virtual list */

	    if (blist->isVirtual())
	    {
		vBases.append(new BaseClass(bclass, TRUE,
					    blist->getVisibility()));
	    }
	    else
		/* add to normal list */
		bases.append(new BaseClass(bclass, FALSE,
					   blist->getVisibility()));

	    /* Now add in its virtual bases (if any) also */

	    while (dblist)
	    {
		vBases.append(new BaseClass(dblist->getClass(), TRUE,
					    dblist->getVisibility()));

		dblist = dblist->getNext();
	    }

	    /* And on to the next base class */

	    blist = blist->getNext();
	}
    }

    vBaseList = vBases.getNext();		/* throw away dummy vbase */
    baseList = bases.getNext();		/* throw away dummy base */
}

void ClassType::printClientSpecials ( ostream& s )
{
    BaseClass *blist = vBaseList;

    /* Special protected constructor */

    s << "\n";
    printTemplatePrefix(s);
    s << typeName(CLIENT_DECL) << "::" << classClientName;
    s << " ( const ClientRpcManager& manager, const RpcBuffer& phv )";
    s << ": ";

    /* initialise virtual bases */

    while (blist)
    {
	ClassType *cl = blist->getClass();
	
	s << cl->typeName(CLIENT_DECL) << "(manager, RpcBuffer(phv,";
	s << classHashval << "))";
	blist = blist->getNext();
	s << ", ";
    }

    /* initialise non-virtual bases */

    blist = baseList;

    while (blist)
    {
	ClassType *cl = blist->getClass();
	
	s << cl->typeName(CLIENT_DECL) << "(manager, RpcBuffer(phv,";
	s << classHashval << "))";
	blist = blist->getNext();
	s << ", ";
    }
    
    s << "_clientManager(manager), _myHashVal(phv)";

    s << "\n{\n_myHashVal << " << classHashval << " << -1;\n}\n";

    if ((classDtor == 0) || (classDtor->getVisibility() != CLASS_PUBLIC))
    {
	s << "\n";
	printTemplatePrefix(s);
	s << typeName(CLIENT_DECL) << "::~" << classClientName;
	s << " ( )\n{\n}\n";
    }
}


/*
 * Only gets called for full class declarations so no need to check
 * fullDecl pointer
 */

void ClassType::printClientTypedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;
    
    if (classStubLoc == STUB_REMOTE)
    {
	if (GlobalInfo::getOutputStyle() != SHADOWS_STYLE)
	{
	    s << "\n#ifndef " << className.touppercase() << "_SERVER\n";
	    s << "#undef " << className << "\n";
	    s << "#define " << className << " ";
	    s << classClientName << "\n";
	    s << "#endif\n";
	}
	
        s << "\n";

	printClassHeader(s, CLIENT_DECL);

	s << "\n{\n";
	
	/* Do public member functions only */
	
	s << "public:\n";

	if (classMarshallOpts == STUB_AUTOMARSHALL)
	{
	    if (hasVirtuals)
	    {
		s << "\tvirtual void marshall (RpcBuffer&) const;\n";
		s << "\tvirtual void unmarshall (RpcBuffer&);\n";
	    }
	    else
	    {
		s << "\tvoid marshall (RpcBuffer&) const;\n";
		s << "\tvoid unmarshall (RpcBuffer&);\n";
	    }
	    s << "\n";
	}

	while ((decl = classMembers->getOneMember(i++)))
	{
	    if (decl->isTypedef() ||
		((decl->getVisibility() == CLASS_PUBLIC) &&
		 (decl->getRemote() == STUB_REMOTE)))
	    {
		Type dkind = decl->getType()->kind();

		if ((dkind == FUNCTION_TYPE) || decl->isTypedef())
		{
		    s << "\t";
		    decl->printDecl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
		}
		else
		{
		    ostrstream errorStream;

		    errorStream << "ignoring non-function public member ";
		    errorStream << className << "::";
		    errorStream << decl->getName();
		    error(errorStream);
		}
	    }
	}

	s << "protected:\n";
	s << "\t" << classClientName << "(const ClientRpcManager&, const RpcBuffer&);\n";
	if (classDtor == 0)
	{
	    /* generate default */
	    s << "\tvirtual ~" << classClientName << "();\n";
	} else
	    if (classDtor->getVisibility() == CLASS_PROTECTED)
	    {
		s << "\t";
		classDtor->printDecl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
	    }

	s << "private:\n";

	if (classDtor != 0 &&
	    (classDtor->getVisibility() == CLASS_PRIVATE))
	{
	    s << "\t";
	    classDtor->printDecl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
	}

	/* State variables */

	s << "\tClientRpcManager _clientManager;\n";
	s << "\tRpcBuffer _myHashVal;\n";

	s << "}\n";
    }
    else
    {
	printAsTypedef(s, NORMAL_DECL);
    }
}

void ClassType::printIdlTypedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;
    
    printClassHeader(s, IDL_DECL);
    
    s << "\n{\n";
	
    /* Do public member functions only */
	
    while ((decl = classMembers->getOneMember(i++)))
    {
	if (decl->isTypedef() ||
	    ((decl->getVisibility() == CLASS_PUBLIC)))
	{
	    Type dkind = decl->getType()->kind();
	    
	    if ((classKind != CLASS_TYPE) ||
		(dkind == FUNCTION_TYPE) || decl->isTypedef())
	    {
		s << "\t";
		decl->printDecl(s, IDL_DECL, ";\n", FALSE, TRUE);
	    }
	    else
	    {
		ostrstream errorStream;

		errorStream << "ignoring non-function public member ";
		errorStream << className << "::";
		errorStream << decl->getName();
		error(errorStream);
	    }
	}
    }

    s << "}\n";
}

void ClassType::printServerSpecials ( ostream& s )
{
    Declaration *decl = 0;
    TypeDescriptor *type = 0;
    int declnum = 0;
    FctType destructor = FctType(this, 0, 0, 0, DESTRUCTOR);
    
    s << "\n";
    printTemplatePrefix(s);
    s << typeName(SERVER_DECL) << "::";
    s << classServerName << " () : ";
    s << "theRealObject(0),";
    s << "_constructArgs_(),";
    s << "_constructOpcode_(0),";
    s << "_constructRefCount_(0),";
    s << "_constructResult_()";
    s << "\n{}\n";

    s << "\n";
    printTemplatePrefix(s);
    s << typeName(SERVER_DECL) << "::~";
    s << classServerName << " ()\n{\n";

    /* only delete if destructor visible */
    decl = classMembers->lookFor(cppLinkage->mangle(&destructor), FUNCTION_TYPE, INHERITED_CONTEXT);
    if ((decl != 0) && (decl->getVisibility() == CLASS_PUBLIC))
    {
	s << "if (theRealObject != 0)\n";
	s << "delete theRealObject;\n";
    }
    s << "}\n";

    /* now produce the operation 'serve' */

    s << "\n";
    printTemplatePrefix(s);
    s << "void " << typeName(SERVER_DECL) << "::serve ( int argc, char **argv )\n{\n";
    s << "ServerRpc ServerRPC;\n";
    s << "Int32 opcode, errcode;\n\n";
    s << "ServerRPC.initialise(argc, argv);\n\n";
    s << "for (;;)\n{\n";
    s << "RpcBuffer work;\n";
    s << "RpcBuffer result;\n\n";
    s << "ServerRPC.getWork(opcode, work);\n";
    s << "errcode = dispatchToClass(theRealObject, opcode, work, result);\n";
    s << "ServerRPC.sendResult(errcode, result);\n";
    s << "}\n";
    s << "}\n";

    /* Now the code for 'dispatchToClass' */

    s << "\n";
    printTemplatePrefix(s);
    s << "Int32 " << typeName(SERVER_DECL) << "::dispatchToClass ( ";
    s << typeName(SERVERIMPLEMENTATION_DECL) << " * theObject, ";
    s << "Int32 funcCode, RpcBuffer& work, RpcBuffer& result)\n{\n";
    s << "Int32 classCode;\n\nwork >> classCode;\n\n";
    s << "switch (classCode)\n{\n";

    s << "case -1:\n";
    s << "return dispatchToOper(theObject, funcCode, work, result);\n";

    /* This is the main loop producing the actual calls */
    {
	BaseClass *blist = vBaseList;

	/* Do virtual bases first */

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << "case " << cl->getClassHashval() << ":\n";
	    s << "return ";
	    s << cl->typeName(SERVER_DECL) << "::dispatchToClass(theObject, funcCode, work, result);\n";
	    blist = blist->getNext();
	}

	/* Now non-virtual bases */

	blist = baseList;

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << "case " << cl->getClassHashval() << ":\n";
	    s << "return ";
	    s << cl->typeName(SERVER_DECL) << "::dispatchToClass(theObject, funcCode, work, result);\n";
	    blist = blist->getNext();
	}

    }
    
    /* Default action if class opcode not understood. */

    s << "default:\n";
    s << "return DISPATCH_ERROR;\n";

    /* Now wrap up finally */

    s << "}\n";
    s << "}\n";

    /* Now the code for 'dispatchToOper' */

    s << "\n";
    printTemplatePrefix(s);
    s << "Int32 " << typeName(SERVER_DECL) << "::dispatchToOper ( ";
    s << typeName(SERVERIMPLEMENTATION_DECL);
    s << " * theObject, Int32 funcCode, RpcBuffer& work, RpcBuffer& result)\n{\n";
    s << "switch (funcCode)\n{\n";

    /* This is the main loop producing the actual calls */

    while ((decl = classMembers->getOneMember(declnum++)))
    {
	errorLocation = decl->getLoc();

        if ((decl->getVisibility() == CLASS_PUBLIC) &&
	    (decl->getRemote() == STUB_REMOTE))
        {
            type = decl->getType();

            if (type->kind() == FUNCTION_TYPE)
            {
                FctType *funcptr;

                funcptr = (FctType *)type;      /* legit cast here */
		
		if (!funcptr->isPure())
		{
		    s << "case " << funcptr->getFuncHashval() << ":\n";
		    
		    s << "return ";
		    s << funcptr->simpleTypeName(SERVER_DECL) << "(theObject, work, result);\n";

		}
            }
        }
    }
    
    s << "default:\n";
    s << "return DISPATCH_ERROR;\n";

    /* Now wrap up finally */

    s << "}\n";
    s << "}\n\n";


}

void ClassType::printServerTypedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;
    
    if (classStubLoc != STUB_NOREMOTE)
    {
	if (GlobalInfo::getOutputStyle() != SHADOWS_STYLE)
	{
	    s << "\n#ifdef " << className.touppercase() << "_SERVER\n";
	    s << "#undef " << className << "\n";
	    s << "#define " << className << " ";
	    s << classImplName << "\n";
	    s << "#endif\n";
	}
	
        s << "\n";

	printClassHeader(s, SERVERIMPLEMENTATION_DECL, FALSE);
	s << ";\n\n";
	
        printClassHeader(s, SERVER_DECL);
	
        s << "\n{\n";
        s << "\npublic:\n";
	
        s << "\t" << classServerName << " ();\n";
        s << "\t~" << classServerName << " ();\n\n";
        s << "\tvoid serve (int, char **);\n";
        s << "\tInt32 dispatchToClass (" << typeName(SERVERIMPLEMENTATION_DECL);
	s << "*, Int32, RpcBuffer&, RpcBuffer&);\n\n";
	
	s << "private:\n";
	s << "\tInt32 dispatchToOper (" << typeName(SERVERIMPLEMENTATION_DECL);
	s << " *,Int32, RpcBuffer&, RpcBuffer&);\n\n";
	
        while ((decl = classMembers->getOneMember(i++)))
        {
            if ((decl->getVisibility() == CLASS_PUBLIC) &&
		(decl->getRemote() == STUB_REMOTE))
            {
                if (decl->getType()->kind() == FUNCTION_TYPE)
                {
                    s << "\t";
                    decl->printDecl(s, SERVER_DECL);
                }
            }
        }
	
        s << "\n\t" << typeName(SERVERIMPLEMENTATION_DECL) << " *theRealObject;\n";
	
	s << "\tRpcBuffer _constructArgs_;\n";
	s << "\tInt32 _constructOpcode_;\n";
	s << "\tInt32 _constructRefCount_;\n\tRpcBuffer _constructResult_;\n";
	s << "}";
    }
}

void ClassType::printShadowsTypedef ( ostream& s )
{
    int i = 0;
    Declaration *decl;

    s << "#ifndef STUB\n";
    
    printClassHeader(s, SHADOWS_DECL);
    
    s << "\n{\n";
	
    /* Do public member functions only */
	
    while ((decl = classMembers->getOneMember(i++)))
    {
	if (decl->isTypedef() ||
	    ((decl->getVisibility() == CLASS_PUBLIC)))
	{
	    Type dkind = decl->getType()->kind();
	    
	    if ((dkind == FUNCTION_TYPE) || decl->isTypedef())
	    {
		s << "\t";
		decl->printDecl(s, SHADOWS_DECL, ";\n", FALSE, TRUE);
	    }
	    else
	    {
		ostrstream errorStream;

		errorStream << "ignoring non-function public member ";
		errorStream << className << "::";
		errorStream << decl->getName();
		error(errorStream);
	    }
	}
    }

    s << "protected:\n";
    s << "\t" << className << "(const " << classImplName
      << "*, const " << classClientName << "*);\n";
    if (classDtor == 0)
    {
	/* generate default */
	s << "\tvirtual ~" << classClientName << "();\n";
    } else
	if (classDtor->getVisibility() == CLASS_PROTECTED)
	{
	    s << "\t";
	    classDtor->printDecl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
	}
    
    s << "private:\n";

    if (classDtor != 0 &&
	(classDtor->getVisibility() == CLASS_PRIVATE))
    {
	s << "\t";
	classDtor->printDecl(s, CLIENT_DECL, ";\n", FALSE, TRUE);
    }

    /* State variables */

    s << "\t" << classImplName << "* _local;\n";
    s << "\t" << classClientName << "* _remote;\n";

    s << "}\n";
    s<< "#endif\n";
}
 
Boolean ClassType::produceClientCode ( ostream& codeStream )
{    
    if (classFullDecl)
	return classFullDecl->produceClientCode(codeStream);
    else
    {
	Boolean stubOk = TRUE;
	Declaration *decl;
	int i = 0;
	
	if (classStubLoc != STUB_NOREMOTE)
	{
	    codeStream << "#include <StubGen/ClientRpc.h>\n";

	    codeStream << "\n#include \""
		        << GlobalInfo::getCurrentIf()->getHeaderFname()
			<< "\"\n";

	    printClientSpecials (codeStream);

	    while ((decl = classMembers->getOneMember(i++)))
	    {
		errorLocation = decl->getLoc();
		
		if ((decl->getVisibility() == CLASS_PUBLIC) &&
		    (decl->getRemote() == STUB_REMOTE))
		{
		    if (decl->getType()->kind() == FUNCTION_TYPE)
		    {
			printTemplatePrefix(codeStream);
			stubOk = decl->produceStubCode(codeStream, CLIENT_DECL);
		    }
		}
	    }
	} else
	    stubOk = FALSE;

	return stubOk;
    }
}

Boolean ClassType::produceServerCode ( ostream& codeStream )
{
    if (classFullDecl)
	return classFullDecl->produceServerCode(codeStream);
    else
    {
	Boolean stubOk = TRUE;
	Declaration *decl;
	int i = 0;
	
	if (classStubLoc != STUB_NOREMOTE)
	{
	    codeStream << "#include <StubGen/ServerRpc.h>\n\n";

	    codeStream << "#define " << className.touppercase() << "_SERVER";
	    codeStream << "\n\n#include \""
		        << GlobalInfo::getCurrentIf()->getHeaderFname()
			<< "\"\n";
	    
	    printServerSpecials (codeStream);

	    while ((decl = classMembers->getOneMember(i++)))
	    {
		if ((decl->getVisibility() == CLASS_PUBLIC) &&
		    (decl->getRemote() == STUB_REMOTE))
		{
		    if (decl->getType()->kind() == FUNCTION_TYPE)
		    {
			errorLocation = decl->getLoc();
			if (decl->getRemote() != STUB_NOREMOTE)
			{
			    printTemplatePrefix(codeStream);
			    
			    stubOk = decl->produceStubCode(codeStream, SERVER_DECL);
			    codeStream << "\n";
			}
			else
			{
			    ostrstream errorStream;

			    stubOk = FALSE;
			    errorStream << "server stub generation impossible for "
				         << decl->getType()->typeName(NORMAL_DECL);
			    error(errorStream);
			}
		    }
		}
	    }
	} else
	    stubOk = FALSE;

	return stubOk;
    }
}
 
Boolean ClassType::produceShadowsCode ( ostream& codeStream )
{    
    if (classFullDecl)
	return classFullDecl->produceShadowsCode(codeStream);
    else
    {
	Boolean stubOk = TRUE;
	Declaration *decl;
	int i = 0;
	
	if (classStubLoc != STUB_NOREMOTE)
	{
	    codeStream << "#include <StubGen/ClientRpc.h>\n";

	    codeStream << "\n#include \""
		        << GlobalInfo::getCurrentIf()->getHeaderFname()
			<< "\"\n";

	    printClientSpecials (codeStream);

	    while ((decl = classMembers->getOneMember(i++)))
	    {
		errorLocation = decl->getLoc();
		
		if ((decl->getVisibility() == CLASS_PUBLIC) &&
		    (decl->getRemote() == STUB_REMOTE))
		{
		    if (decl->getType()->kind() == FUNCTION_TYPE)
		    {
			printTemplatePrefix(codeStream);
			stubOk = decl->produceStubCode(codeStream, SHADOWS_DECL);
		    }
		}
	    }
	} else
	    stubOk = FALSE;

	return stubOk;
    }
}

#ifdef NO_INLINES
#  define CLASST_CC_
#  include "ClassT.n"
#  undef CLASST_CC_
#endif
