/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncT.cc,v 1.50 1993/11/02 12:36:57 ngdp Exp $
 */

/*
 * Stub generator function definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
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

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef POINTERT_H_
#  include "PointerT.h"
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

static const char RCSid[] = "$Id: FuncT.cc,v 1.50 1993/11/02 12:36:57 ngdp Exp $";

/*
 * PUBLIC operations
 */

/*
 * Construct a function type. Note that at the time we construct this
 * in the parse we typically do not know the name of the function - 
 * it gets added later
 */

FctType::FctType ( ClassType *inclass,
		   DeclarationList *args, TypeDescriptor *fQuals,
		   FunctionType ftyp )
                 : TypeDescriptor(FUNCTION_TYPE),
		   argsPending(args),
		   ftype(ftyp),
		   funcBody(0),
		   funcClientName(NullString),
		   funcClientVisibility(STUB_PENDING),
		   funcFrozen(FALSE),
		   funcHashval(0),
		   funcImplName(NullString),
		   funcInline(FALSE),
		   funcIsmemberOf(inclass),
		   funcName(NullString),
		   funcPure(FALSE),
		   funcQuals(fQuals),
		   funcReturnType(0),
		   funcFakeReturn(0),
		   funcServerName(NullString),
		   funcSignature(NullString),
		   funcVirtual(FALSE),
		   hasResultArgs(FALSE),
		   hasVarArgs(FALSE),
		   hasNonVoidResult(FALSE)
{
#ifdef DEBUG
    debugStream << "FctType::FctType (" << (void *)inclass
		<< "," << (void *)args << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

FctType::~FctType ()
{
}

void FctType::makePure ()
{
    funcPure = TRUE;
}

Boolean FctType::isPure () const
{
    return funcPure;
}

/*
 * Function initialisers are treated as if they added the inline attribute
 */

Boolean FctType::addBody ( Expression *body )
{
    funcInline = TRUE;
    if (funcBody == 0)
    {
	funcBody = body;
    }
    else
	error("function already has a definition");

    return TRUE;
}

Boolean FctType::addFuncModifier ( FuncModifier newFm )
{
#ifdef DEBUG
    debugStream << "FctType::addFuncModifier("  << (void *)this
		<< "," << newFm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (newFm == FUNC_INLINE)
        funcInline = TRUE;
    else
    {
	if (funcIsmemberOf)
	    funcIsmemberOf->addVirtual();
	
        funcVirtual = TRUE;
    }

    return TRUE;
}

Boolean FctType::addName ( const String& fname )
{
#ifdef DEBUG
    debugStream << "FctType::addName(" << (void *)this 
		<< ", " << fname << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (funcName == NullString)
    {
	funcName = fname;
	funcClientName = fname;
	funcServerName = fname;
	funcImplName = fname;

	if (ftype == CONSTRUCTOR)
	    funcSignature = "_Ct";
	else
	    if (ftype == DESTRUCTOR)
	    {
		funcSignature = "__dt";
	    }
	    else
		funcSignature = fname;

	if (fname(0, 8) == "operator")
	{
	    ftype = OPER;
	    funcServerName = "op__";
	}

	funcSignature += "__";

	if (funcIsmemberOf != 0)
	{
	    funcSignature += funcIsmemberOf->typeName(NORMAL_DECL);
	    if ((ftype == CONSTRUCTOR) || (ftype == DESTRUCTOR))
	    {
		funcName = funcIsmemberOf->simpleTypeName(NORMAL_DECL);
		funcImplName = NullString;
		funcClientName = NullString;
	    }
	}

	funcSignature += "F";
    
	if (argsPending != 0)
	{
	    setArgList(argsPending);
	    delete argsPending;
	    argsPending = 0;
	}
	else
	    funcSignature += "v";

    }
    return TRUE;
}

Boolean  FctType::addStubModifier ( StubModifier sm )
{
#ifdef DEBUG
    debugStream << "FctType::addStubModifier("  << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    switch (sm)
    {
    case STUB_REMOTE:
    case STUB_NOREMOTE:
    case STUB_NOCLIENT:
	if (funcClientVisibility == sm)
	    return TRUE;
	else
	    if (funcClientVisibility == STUB_PENDING)
	    {
		if ((sm == STUB_REMOTE) && (ftype == OPER))
		{
		    caution("some operator functions cannot be remotely called");
		}
		funcClientVisibility = sm;
		return TRUE;
	    } else
	    {
		errorStream << "Stub modifier '" << sm << "' "
			    << "conflicts with earlier modifier '"
			    <<  funcClientVisibility << "'";
		error(errorStream);
	    }
	
    default:
	break;
    }
    
    return TypeDescriptor::addStubModifier(sm);
}

Boolean FctType::attachBaseType ( TypeDescriptor *baseType )
{
#ifdef DEBUG
    debugStream << "FctType::attachBaseType(" << (void *)this << ", " 
		<< (void *)baseType << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (funcReturnType)
	funcReturnType->attachBaseType(baseType);
    else
	funcReturnType = baseType;

    return TRUE;
}

TypeDescriptor *FctType::combineTypes ( TypeDescriptor *nt )
{
#ifdef DEBUG
    debugStream << "FctType::combineTypes(" << (void *)nt << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (!funcFrozen)
    {
	if (funcReturnType == 0)
	    funcReturnType = nt;
	else
	    funcReturnType = funcReturnType->combineTypes(nt);
    }
    else
	error("(internal) function return type cannot be adjusted further");

    return this;

}

Boolean FctType::freezeType ()
{
#ifdef DEBUG
    debugStream << "FctType::freezeType(" << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    register Declaration *decl;
    int parnum = 0;
    ostrstream convStrm;
    
    if (funcFrozen)
	return TRUE;

    /* Check return type and freeze it */

    if (ftype == NORMAL && funcReturnType == 0)
    {
	errorStream << "return type defaulting to 'int'";
	caution(errorStream);
	funcReturnType = new IntType();
    }

    if (funcReturnType)
    {
	funcReturnType->freezeType();
 	
	if (funcReturnType->simpleTypeName(NORMAL_DECL) != "void")
	{
	    hasNonVoidResult = TRUE;
	}
    }
    

    /* freeze argument list */

    while ((decl = argList.getOneMember(parnum++)) != 0)
    {
	decl->freezeDecl();
    }		

    /* Set up stub specific options */

    if (funcClientVisibility == STUB_PENDING)
    {
	if (funcIsmemberOf == 0)
	    funcClientVisibility = STUB_NOREMOTE;
	else
	    if ((ftype != OPER) || (isTransferable()))
	    {
		funcClientVisibility = funcIsmemberOf->defaultLocation();
	    }
	    else
	    {
		errorStream << "stub code production for \"";

		printInstanceDecl(errorStream,NORMAL_DECL,FALSE,FALSE,TRUE);
		errorStream << "\" not supported";
	
		error(errorStream);
	
		if (funcIsmemberOf != 0)
		    funcClientVisibility = STUB_NOCLIENT;
		else
		    funcClientVisibility = STUB_NOREMOTE;
	    }
    }
    
    if ((funcReturnType) && (funcClientVisibility == STUB_REMOTE))
    {
	String tName = funcReturnType->typeName(NORMAL_DECL);

	if ((tName != "void") && (tName != "const void") &&
	    !funcReturnType->isTransferable())
	{
	    errorStream << "return type of \"";
	    printInstanceDecl(errorStream, NORMAL_DECL, FALSE, FALSE, TRUE);
	    errorStream << "\" cannot be marshalled\n";
	    error(errorStream);
	}
    }

    /* checkout arglist marshalling */

    parnum = 0;
    
    while ((decl = argList.getOneMember(parnum++)) != 0)
    {
	TypeDescriptor *td = decl->getType();	
	String name = decl->getName();
	Boolean canMarshall = TRUE;

	decl->freezeDecl();
		
	switch(decl->getTransfer())
	{
	case STUB_ARG_INOUT:
	case STUB_ARG_OUT:
	    hasResultArgs = TRUE;	/* BEWARE deliberate fall through */
	case STUB_ARG_IN:
	    canMarshall = td->isTransferable();
	    break;
	default:
	    canMarshall = FALSE;
	    break;
	}

	if ((funcClientVisibility == STUB_REMOTE) && !canMarshall)
	{
	    errorStream << "argument " << parnum << " of \"";
	    printInstanceDecl(errorStream, NORMAL_DECL, FALSE,FALSE,TRUE);
	    errorStream << "\" cannot be marshalled\n";
	    error(errorStream);
	}
	
	if (name == "...")
	    hasVarArgs = TRUE;
    }

    if (funcPure && !funcVirtual)
    {
	errorStream << "only virtual functions can be pure";
	error(errorStream);
	funcPure = FALSE;
    }
    funcHashval = hashIt(funcSignature);
    
    convStrm << funcServerName << (funcHashval) << '\0';

    /* The call on str() prevents the buffer being deleted */

    funcServerName = convStrm.str();

    /* And this allows it to be deleted after all ! */

    convStrm.rdbuf()->freeze(0);

    if ((funcIsmemberOf != 0) &&
	((ftype == CONSTRUCTOR) || (ftype == DESTRUCTOR)))
    {
	funcClientName = funcIsmemberOf->simpleTypeName(CLIENT_DECL);
	funcImplName = funcIsmemberOf->simpleTypeName(SERVERIMPLEMENTATION_DECL);
    }

    funcFrozen = TRUE;
    return TRUE;
}

Boolean FctType::isFundamental () const
{
    return FALSE;
}

/*
 * Note we ignore the incoming first parameter
 */

String FctType::massageName ( const String&, DeclStyle ds ) const
{
    return simpleTypeName(ds);
}

/*
 * postname handles the actual argument list.
 */

String FctType::postName ( DeclStyle ds ) const
{
    char buffer[1024];
    ostrstream s(buffer,1024);
    
    switch (ds)
    {
    case CLIENT_DECL:
	printArgList(s, ds, TRUE, FALSE, TRUE);
	break;
    case SERVER_DECL:
	break;
    case SERVERIMPLEMENTATION_DECL:
	printArgList(s, ds, funcInline ? FALSE:TRUE, FALSE, FALSE);
	break;
    case NORMAL_DECL:
	printArgList(s, ds, funcInline ? FALSE:TRUE, FALSE, FALSE);

    }

    if (funcQuals)
    {
	s << " " << funcQuals->preName(NORMAL_DECL)
	  << funcQuals->postName(NORMAL_DECL);
    }

    if (funcPure)
	s << " = 0";

    if (funcReturnType)
	s << funcReturnType->postName(ds);

    s << ends;
    
    return buffer;
}

String FctType::preName ( DeclStyle, Boolean, Boolean ) const
{
    String s;

    if (ftype == DESTRUCTOR)
	s = "~";
    else
	if (ftype == CONVERSION)
	    s = "operator ";

    return s;
}


String FctType::signature () const
{
    return funcSignature;
}

String FctType::simpleTypeName ( DeclStyle ds ) const
{ 
    switch (ds)
    {
    case SERVER_DECL:
	return funcServerName;
    case SERVERIMPLEMENTATION_DECL:
	return funcImplName;
    case CLIENT_DECL:
	return funcClientName;
    case NORMAL_DECL:
    default:
	return funcName;
    }
}

StubModifier FctType::defaultLocation () const
{
#ifdef DEBUG
    /* 
     * The void * const cast in the next statement should really be to
     * a const void * const but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debugStream << "FctType::defaultLocation(" << (void * const)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return funcClientVisibility;
}

StubModifier FctType::defaultMarshall () const
{
#ifdef DEBUG
    debugStream << "FctType::defaultMarshall(" << (void * const)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return STUB_IGNORE;
}

StubModifier FctType::defaultTransfer () const
{
#ifdef DEBUG
    debugStream << "FctType::defaultTransfer(" << (void * const)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return STUB_ARG_NEITHER;
}

Boolean FctType::isTransferable () const
{
    Declaration *decl = 0;
    int parnum = 0;
    
    if ((hasNonVoidResult) &&
	(funcReturnType != 0) && !(funcReturnType->isTransferable()))
	return FALSE;

    while ((decl = argList.getOneMember(parnum)) != 0) /* iterate over arglist */
    {
        if (decl->getTransfer() == STUB_ARG_NEITHER)
        {
            return FALSE;
        } 
        parnum++;
    }
    return TRUE;
}

Boolean FctType::produceMarshallingCode (ostream&, Direction,
					 const String&, const String&)
{
    return FALSE;
}

Boolean FctType::produceStubCode ( ostream& s, DeclStyle ds )
{
    switch (ds)
    {
    case CLIENT_DECL:
        printClientCode(s);
        break;
    case SERVER_DECL:
        printServerCode(s);
        break;
    case SERVERIMPLEMENTATION_DECL:
    case NORMAL_DECL:
        break;
    }
    return TRUE;
}

ostream& FctType::printInstanceOf ( ostream& s,
				    const String&,
				    DeclStyle ds,
				    Boolean b,
				    Boolean noConst)
{
    switch (ds)
    {
    case SERVER_DECL:
	s << "long " << funcServerName;
	s << "(" << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL) << " *, RpcBuffer&, RpcBuffer&)";
	break;
    case CLIENT_DECL:
    case NORMAL_DECL:
    case SERVERIMPLEMENTATION_DECL:
	printInstanceDecl(s,ds,b,noConst,FALSE);
    }

    return s;
}

/*
 * Warning. Ordering of functions calls here is tricky due to when
 * things are set up by complete/freezeDecl.
 */

void FctType::setArgList ( DeclarationList *d )
{
#ifdef DEBUG
    debugStream << "FctType::setArgList (" << (void *)d
	<< ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    DeclarationListIterator next(d);
    register Declaration *decl;
    long parnum = 0;

    if (!funcFrozen)
    {
	if ((ftype == DESTRUCTOR) && (d != 0))
	    error("destructors have no arguments");
	else
	    while (decl = next())
	    {
		String name = decl->getName();
		TypeDescriptor *typ = decl->getType();

		if ((name == NullString))
		    decl->setName(constructName("_Par_", parnum++));

		/* Add to arg list NOW */

		argList.putIn(decl);

		if (typ != 0)
		    funcSignature += typ->signature();
	    }
    }
    else
	error("(internal) function already has an argument list");
}

void FctType::declareClientLocals ( ostream& s )
{
    TypeDescriptor *type = 0;

    s << "\tRpcBuffer callBuffer(_myHashVal);\n";
    s << "\tRpcBuffer resultBuffer;\n";
    s << "\tRPC_Status rpcStatus = OPER_UNKNOWN;\n";
    s << "\tlong serverStatus = 0;\n\n";

    /*
     * return type (if any). Take care - if the return type is a pointer
     * then it has to be dereferenced to get the actual type to declare and
     * then a pointer to that type returned. Such return values are
     * created via 'new' to ensure that the value persists after the end of
     * the  function
     */

    /* Cfront 2.1 can't cope with && here... */
    if (funcReturnType != 0)
	if (funcReturnType->typeName(NORMAL_DECL) != "void")
	{
	    Boolean useNew = FALSE;
	    Type rv_kind;
	    
	    type = funcReturnType;
	    rv_kind = type->kind();

	    /* Discard 'const' flag if present (otherwise cannot marshall) */

	    s << "\t";

	    if ((rv_kind == CV_TYPE) || (rv_kind == POINTER_TYPE) ||
		(rv_kind == REFERENCE_TYPE))
	    {
		if ((rv_kind == REFERENCE_TYPE))
		    useNew = TRUE;
		
		while ((rv_kind == REFERENCE_TYPE) ||
		       (rv_kind == CV_TYPE))
		{
		    /* do the deref */
		    type = type->deref();
		    rv_kind = type->kind();
		}
	    }

	    /* Set up the fake return type */
	    funcFakeReturn = type;

	    type->printInstanceOf(s, NullString, CLIENT_DECL);

	    if (useNew)
	    {
		s << "& returnedValue = * new ";
		s << type->typeName(CLIENT_DECL);
	    }
	    else
		s << "returnedValue";

	     s << type->defaultInit();

	    s << ";\n";
	}
}

void FctType::doCallToServer ( ostream& s )
{
    s << "\trpcStatus = _clientHandle.Call(" << funcHashval;
    s << ", callBuffer, serverStatus, resultBuffer);\n";
}

void FctType::printArgList ( ostream& s, DeclStyle ds, Boolean noName, 
			     Boolean nodefault, Boolean special ) const
{
    s << " ( ";
    if (argList.noOfEntries() != 0)
    {
	Declaration *decl;
	String name;
	int limit = argList.noOfEntries();
	int i = 0;

	while (decl = argList.getOneMember(i++))
	{
	    name = decl->getName();
	    if (name == "...")
		decl->printDecl(s, NORMAL_DECL, " ", FALSE, nodefault); /* print regardless */
	    else
	    {
		const char *sep = ( i < limit ? ", " : "");

		decl->printDecl(s, ds, sep, noName, nodefault);
	    }
	}
    }
    if (ftype == CONSTRUCTOR && special)
    {
	if (argList.noOfEntries() != 0)
	    s << ", ";
	s << "RpcControl *cRpc ";
	if (!nodefault)
	{
	    String sName = GlobalInfo::getServerName();

	    if (sName == NullString)
		sName = funcServerName;

	    s << "= RpcControl::createInstance(\"";	    
	    s << sName << "\",1)";
	}
    }
    s << ")";
}


/*
 * Pack the argument list (if necessary). Return a Boolean value which
 * indicates whether the par list was packed ok. Arguments are packed via
 * the operator << applied to the rpc buffer. If the type is a pointer
 * or reference special things happen to avoid packing the same object more
 * than once and to preserve referential integrity.
 */

Boolean FctType::packCallArgList ( ostream& s )
{
    Declaration *decl;
    int parnum = 0;
    Boolean allOk = TRUE;
    StubModifier declSm;

    while ((decl = argList.getOneMember(parnum)) != 0) /* iterate over arglist */
    {
	declSm = decl->getTransfer();

        if ((declSm == STUB_ARG_IN) || (declSm == STUB_ARG_INOUT))
        {
            decl->produceMarshallCode(s, IN, "callBuffer");
        } 
	else
        {
            if (allOk)
                allOk = FALSE;
        }
        parnum++;
    }
    return allOk;
}

Boolean FctType::packResultArgList ( ostream& s )
{
    Declaration *decl;
    int parnum = 0;
    StubModifier declSm;

    while ((decl = argList.getOneMember(parnum)) != 0) /* iterate over arglist */
    {
	declSm = decl->getTransfer();
        if ((declSm == STUB_ARG_OUT) || (declSm == STUB_ARG_INOUT))
	{
	    Boolean deleteDummy = FALSE;
	    TypeDescriptor *type, *dummytype;
	    Type argkind;

	    type = decl->getType();
	    argkind = type->kind();

	    if  ((argkind == REFERENCE_TYPE))
	    {
		dummytype = type->deref();
		
		if (dummytype->isFundamental())
		{
		    deleteDummy = TRUE;
		    type = new PointerType(POINTER_TYPE, dummytype);
		}
	    }

            type->produceMarshallingCode(s, IN, decl->getName(), "result");
	    if (deleteDummy)
		delete type;
	}
        parnum++;
    }

    return TRUE;
}

Boolean FctType::printClientCode ( ostream& s )
{
    Boolean funcStubOk = TRUE;
    String className = funcIsmemberOf->typeName(CLIENT_DECL);
    TypeDescriptor *type = 0;
    Boolean wasvirtual = funcVirtual;

    /* If function is pure don't want a definition */

    if (funcPure)
	return TRUE;

    /* Check that we can produce stub code for this operation */

    if ((ftype == CONVERSION) ||
        hasVarArgs)
    {
        errorStream << "stub code production for ";
	if (ftype == CONVERSION)
            errorStream << "conversion operation ";

	if (funcIsmemberOf)
	    errorStream << funcIsmemberOf->typeName(NORMAL_DECL) << "::";
	if (ftype == OPER)
            errorStream << "operator ";

	errorStream << typeName(NORMAL_DECL);
	errorStream << " not supported";
	
        if(hasVarArgs)
            errorStream << " due to variable length argument list";

        error(errorStream);
        return FALSE;
    }

    /* Determine if we have a genuine return type */

    if (funcReturnType)
    {
	if (!funcReturnType->isFundamental())
	{
	    errorStream << "reference/pointer return value of \"";
	    printInstanceDecl(errorStream, NORMAL_DECL,FALSE,FALSE,TRUE);
	    errorStream << "\" may leak memory";
	    caution(errorStream);
	}
	
	if (hasNonVoidResult)
	{
	    if (!funcReturnType->isTransferable())
	    {
		return FALSE;
	    }
	}
    }

    /* OK - First produce the function header */

    s << "\n";

    funcVirtual = FALSE;		/* temporarily remove status */

    if (funcReturnType)
    {
	s << funcReturnType->preName(CLIENT_DECL) << " ";
    }

    funcVirtual = wasvirtual;		/* restore virtual status */
    s << className << "::";		/* class :: */

    switch (ftype)
    {
    case DESTRUCTOR:
        s << "~";			/* NOTE fall through */
    case CONSTRUCTOR:
	s << funcIsmemberOf->simpleTypeName(CLIENT_DECL);
	break;
    default:
	s << funcName;
    }

    printArgList(s, CLIENT_DECL, FALSE, TRUE, TRUE);	/* augmented argument list */

    if (funcQuals)
    {
	s << " " << funcQuals->typeName(CLIENT_DECL);
    }

    if (ftype == CONSTRUCTOR)
    {
	/* Constructor - generate member init list */

	BaseClass *blist = funcIsmemberOf->getVbaseList();

	s << ": ";

	/* Do virtual bases first */

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << cl->typeName(CLIENT_DECL) << "(*cRpc, RpcBuffer())";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* Now non-virtual bases */

	blist = funcIsmemberOf->getNVBaseList();

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << cl->typeName(CLIENT_DECL) << "(*cRpc, RpcBuffer())";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* finally class members */

	s << "_clientHandle (*cRpc), _myHashVal(-1)";
    }

    /* Now produce the function body */

    s << "\n{\n";

    /* First the declarations */

    declareClientLocals(s);
    
    if (ftype == CONSTRUCTOR)
	s << "\tcRpc->destroy();\n\n";
    

    if (argList.noOfEntries() != 0)     /* pack up any args */
    {
        funcStubOk = packCallArgList(s);
    }

    s << "\n";

    doCallToServer(s);

    /* General function processing. Unpack any return args and the */
    /* return type if it exists */

    s << "\n\tif (rpcStatus == OPER_DONE && serverStatus == 0)\n";
    s << "\t{\n";

    if (hasResultArgs || hasNonVoidResult)
    {
        if (hasNonVoidResult)
        {
            s << "\t";
	    funcFakeReturn->produceMarshallingCode(s, OUT, "returnedValue", "resultBuffer");
        }
        if (hasResultArgs)
            unpackArgList(s);
    }

    s << "\t}\n\telse\n\t\t_clientHandle.rpcAbort();\n\n";

    /* If op has return value, return it now */

    if (hasNonVoidResult)
    {
        Type returnkind;
        int bracelevel = 1;

        type = funcReturnType;
        s << "\treturn (";
        returnkind = funcReturnType->kind();
#if 0
        while ((returnkind == POINTER_TYPE) ||
               (returnkind == REFERENCE_TYPE)) /* ensure correct return type */
        {
            if (returnkind == POINTER_TYPE)
            {
                s << "&(";               /* ie address of local var */
                bracelevel++;
            }
            type = type->deref();
            returnkind = type->kind();
        }
#endif
        s << "returnedValue";
        for (int i = 0; i < bracelevel; i++)
            s << ")";
        s << ";\n";
    }

    /* Now the operation end brace, and we're done */

    s << "}\n";

    return funcStubOk;

}

void FctType::printInstanceDecl ( ostream& s, DeclStyle ds, Boolean elab,
				  Boolean noConst, Boolean qualify )
{     
    if (funcInline && ds != CLIENT_DECL)
	s << "inline ";
    if (funcVirtual)
	s << "virtual ";
    
    if (funcReturnType != 0 && ftype != CONVERSION)
    {
	s << funcReturnType->preName(ds,elab,noConst);
	s << " ";
    }

    s << preName(ds,noConst);

    if (qualify && (funcIsmemberOf != 0))
	s << funcIsmemberOf->typeName(NORMAL_DECL) << "::";

    s << simpleTypeName(ds);
    s << postName(ds);
}

/*
 * Produce stub code for server side operations. Each function first creates
 * local variables for the arguments to the real function and unpacks them
 * from the supplied RpcBuffer. The 'real' call is then made and the result
 * RpcBuffer constructed and returned.
 * If the parameters were references then special things happen. In particular
 * references to fundamental types get sent as pointers - so have to be declared
 * as such here. References to pointers do not need such special treatment and so get
 * left alone
 */

Boolean FctType::printServerCode ( ostream& s )
{
    Boolean realreturn = (funcReturnType != 0 ? TRUE : FALSE);
    Boolean cleanup = FALSE;
    int parnum = 0;
    Declaration *decl, *argdecl;
    Type argkind;
    TypeDescriptor *type = 0;
    
    /* No code possible for pure functions */
    
    if (funcPure)
    {
	errorStream << "No stub code produced for pure function \"";
	printInstanceDecl(errorStream, NORMAL_DECL,FALSE,FALSE,TRUE);
	errorStream << "\"";
	warning(errorStream);
	return TRUE;
    }
    
    
    if (realreturn)
	if (funcReturnType->typeName(NORMAL_DECL) == "void")
	    realreturn = FALSE;
    
    if ((ftype == CONVERSION) || hasVarArgs)
        return FALSE;
    
    /* First produce the function header */
    
    s << "long ";                       /* return type */
    s << funcIsmemberOf->typeName(SERVER_DECL) << "::"; /* class :: */
    s << funcServerName << " (";
    s << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL);
    s << " * theObject, RpcBuffer& work, RpcBuffer& result)";
    
    /* Now produce the function body */
    
    s << "\n{\n";

    s << "\tlong errCode = OPER_DONE;\n";

    if (ftype == CONSTRUCTOR)
    {
	s << "\tint constructedHere = 0;\n";
	s << "\tRpcBuffer savedArgs = work;\n";
    }
    
    /* Produce the local variables for the parameters */
    
    if (argList.noOfEntries() != 0)
    {
	Boolean deleteDummy;
	TypeDescriptor *dummytype = 0;
        Type argkind;
        String name;

	parnum = 0;

        while ((argdecl = argList.getOneMember(parnum++)) != 0)
        {
            s << "\t";
	    deleteDummy = FALSE;
            dummytype = type = argdecl->getType();
            name = argdecl->getName();
            argkind = type->kind();

	    if (argkind == TYPE_TYPE)
	    {
		argkind = type->deref()->kind();
	    }
		
	    if ((argkind == POINTER_TYPE) || (argkind == REFERENCE_TYPE))
		cleanup = TRUE;

	    /* Treat references to fundamental types as pointers in server code! */

	    if (argkind == REFERENCE_TYPE)
	    {
		TypeDescriptor *t = type->deref();

		if (t->isFundamental())
		{
		    deleteDummy = TRUE;
		    dummytype = new PointerType(POINTER_TYPE, t);
		}
		else
		    dummytype = t;
	    }
		
            dummytype->printInstanceOf(s, name, SERVERIMPLEMENTATION_DECL, FALSE,TRUE);
	    s << dummytype->defaultInit();
	    
            s << ";\n";
	    
            dummytype->produceMarshallingCode(s, OUT, name, "work");
	    if (deleteDummy)
		delete dummytype;
        }
    }
    s << "\n";
    
    if (ftype == DESTRUCTOR)            /* real object destruction */
    {  
	s << "\tif (--constructRefCount == 0)\n\t{\n";
        s << "\t\tdelete theObject;\n";
        s << "\t\ttheObject = 0;\n";
        s << "\t\tconstructOpcode = 0;\n";
	s << "\t}\n";
    } else
    {
        if (ftype == CONSTRUCTOR)       /* requires realobject construction */
        {
	    s << "\tif ((constructRefCount > 0) && "
	      << "((constructOpcode != " << funcHashval << ") || "
	      << "(constructArgs != savedArgs)))\n";
	    
	    s << "\t\terrCode = OPER_NOTDONE;\n";
	    s << "\telse\n\t{\n";
	    s << "\t\tconstructRefCount++;\n";
	    s << "\t\tif (constructRefCount > 1)\n";
	    s << "\t\t{\n\t\t\tresult = constructResult;\n";
	    s << "\t\t}\n";
	    s << "\t\telse\n\t\t{\n";
	    s << "\t\t\tconstructedHere = 1;\n";
            s << "\t\t\ttheRealObject = new " << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL);
        } else
        {                               /* normal operation */
            s << "\t";
            if (realreturn)
	    {
                funcReturnType->printInstanceOf(s, "returnedValue", NORMAL_DECL, FALSE);
		s << " = ";
	    }
	    
            s << "theObject->" << funcName;
        }
	
        s << "(";                       /* open parlist */
	
	parnum = 0;
        while (decl = argList.getOneMember(parnum++))
        {
            type = decl->getType();
            argkind = type->kind();

	    /* REMEMBER. References to fundamental types passed as pointers => need deref */

            if ((argkind == REFERENCE_TYPE) && (type->deref()->isFundamental()))
            {
		s << "*";           /* ie dereference address of local var */
            }
            s << decl->getName();
            if (parnum < argList.noOfEntries())
                s << ", ";
        }
        s << ");";
    }
    s << "\n";
    
    /* Call has been made, see if any arguments and/or a return value is */
    /* indicated  */
    
    if (hasResultArgs || realreturn)
    {
        if (realreturn)
        {
	    funcReturnType->produceMarshallingCode(s, IN, "returnedValue", "result");
        }
        if (hasResultArgs)
            packResultArgList(s);
    }
    
    if (ftype == CONSTRUCTOR)
    {
	s << "\n\t\t}\n\t}\n\tif (constructedHere)\n\t{\n";
	s << "\t\tconstructResult = result;\n";
	s << "\t\tconstructArgs = savedArgs;\n";
	s << "\t\tconstructOpcode = " << funcHashval << ";\n";
	s << "\t}\n";
    }
    
    /* Cleanup up any allocated variables */

    if (cleanup)
    {
        Type argkind;
        String name;

	parnum = 0;
	s << "\n\tlong dummy = 0;\n";
	s << "\n\twork.clearMap();\n\n";	
        while ((argdecl = argList.getOneMember(parnum++)) != 0)
        {
            type = argdecl->getType();
            name = argdecl->getName();
            argkind = type->kind();

	    if (argkind == TYPE_TYPE)
	    {
		argkind = type->deref()->kind();
	    }
		
            if ((argkind == REFERENCE_TYPE) || (argkind == POINTER_TYPE))
            {

		s << "\tif(work.addPtr(" << name << ", dummy))\n";
		s << "\t\tdelete " << name << ";\n";
	    }
        }
    }
    s << "\n";
    
    
    s << "\treturn errCode;\n";
    s << "}\n\n";
    return TRUE;
}

/*
 * Unpack the argument list (if necessary). Inverse of packarglist that
 * uses the operator >> of rpc buffers to retreive the value.
 */

void FctType::unpackArgList ( ostream& s )
{
    Declaration *decl;
    StubModifier declSm;
    int parnum = 0;

    while ((decl = argList.getOneMember(parnum)) != 0) /* iterate over arglist */
    {
	declSm = decl->getTransfer();
        if ((declSm == STUB_ARG_OUT) ||
            (declSm == STUB_ARG_INOUT))
        {
	    s << "\t";
            decl->produceMarshallCode(s, OUT, "resultBuffer");
        }
        parnum++;
    }
}

#ifdef NO_INLINES
#  define FUNCT_CC_
#  include "FuncT.n"
#  undef FUNCT_CC_
#endif
