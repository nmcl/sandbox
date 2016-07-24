/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncT.cc,v 1.33 1996/11/08 14:27:41 ngdp Exp $
 */

/*
 * Stub generator function definition handler
 *
 */

#include <strstream.h>

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

#ifndef INTT_H_
#  include "IntT.h"
#endif

#ifndef VOIDT_H_
#  include "VoidT.h"
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

#ifndef ADCLCNTXT_H_
#  include "ADclCntxt.h"
#endif

#ifndef EDCLCNTXT_H_
#  include "EDclCntxt.h"
#endif

static const char RCSid[] = "$Id: FuncT.cc,v 1.33 1996/11/08 14:27:41 ngdp Exp $";

struct ParamInfo
{
    ParamInfo();
    
    Boolean takeAddr;
    Boolean deref;
    Boolean cast;
    Boolean serverCleanUp;
    Boolean arrayDelete;
    
    TypeDescriptor *clientMarshallType;	/* client type to marshall */
    TypeDescriptor *serverMarshallType;	/* server type to marshall */
    TypeDescriptor *serverDeclType;	/* the type to declare */
};

ParamInfo::ParamInfo ()
		     : takeAddr(FALSE),
		       deref(FALSE),
		       cast(FALSE),
		       serverCleanUp(FALSE),
		       arrayDelete(FALSE),
		       clientMarshallType(0),
		       serverMarshallType(0),
		       serverDeclType(0)
{
}

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
		   DeclarationList *exceptions, FunctionType ftyp )
                 : TypeDescriptor(FUNCTION_TYPE),
		   argInfo(0),
		   argList(new ArgDeclContext(this, (inclass ? inclass->getMemberContext(): 0))),
		   argsPending(args),
		   exceptionInfo(0),
		   exceptionSpec(0),
		   freeResult(FALSE),
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
		   hasNonVoidResult(FALSE),
                   remotelyCallable(TRUE)
{
#ifdef SG_DEBUG
    debugStream << "FctType::FctType (" << (void *)inclass
		<< "," << (void *)args << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    if (exceptions)
    {  
	DeclarationListIterator next(exceptions);
	Declaration *decl;
	int count = 0;

	if (funcIsmemberOf)
	    funcIsmemberOf->exceptionsNeeded();
	
 	exceptionSpec = new ExceptionDeclContext();
	
	while ((decl = next()))
	{
	    if (decl->getName() == NullString)
		decl->setName(constructName("_Ex_", count++));

	    exceptionSpec->putIn(decl);
	}

	delete exceptions;
    }
}

FctType::~FctType ()
{
    if (argList)
	delete argList;

    if (argInfo)
	delete [] argInfo;

    if (exceptionInfo)
	delete [] exceptionInfo;
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
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
    debugStream << "FctType::addName(" << (void *)this 
		<< ", " << fname << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (funcName == NullString)
    {
	/* Check if name is qualified */

	Boolean isQual = FALSE;
	String altName = fname.rindex(':');

	if (altName == NullString)
	    altName = fname;
	else
	{
	    isQual = TRUE;
	    altName = altName(1,0);
	}
	
	funcName = fname;
	funcClientName = altName;
	funcServerName = altName;
	funcImplName = altName;

	if (ftype == CONSTRUCTOR)
	    funcSignature = "_ct";
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

	    if (isQual)
	    {
		String qName = funcIsmemberOf->typeName(NORMAL_DECL,FALSE,TRUE);
		qName += String("::") + altName;
		ostrstream errorStream;

		if (qName != fname)
		{
		    errorStream << "cannot declare member function '"
				<< fname
				<< "' within '"
				<< funcIsmemberOf->typeName(NORMAL_DECL)
				<< "'";
		    
		    error(errorStream);
		}
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
#ifdef SG_DEBUG
    debugStream << "FctType::addStubModifier("  << sm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    ostrstream errorStream;

    switch (sm)
    {
    case STUB_NOCLIENT:
	caution("@NoCLient no longer valid - use @NoRemote");
	sm = STUB_NOREMOTE;
    case STUB_REMOTE:
    case STUB_NOREMOTE:

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
    case STUB_DELETE:
	freeResult = TRUE;
	return TRUE;
    case STUB_NODELETE:
	freeResult = FALSE;
	return TRUE;
    case STUB_NOMARSHALL:
	/* Silently accept this */
	return TRUE;
    default:
	break;
    }
    
    return TypeDescriptor::addStubModifier(sm);
}

Boolean FctType::attachBaseType ( TypeDescriptor *baseType )
{
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
    debugStream << "FctType::freezeType(" << (void *)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    ostrstream convStrm;
    ostrstream errorStream;
    
    if (funcFrozen)
	return TRUE;

    /* Check return type and freeze it */

    if (ftype == CONSTRUCTOR && funcReturnType != 0)
    {
	errorStream << "Constructors and destructors may not have return types";
	error(errorStream);
	funcReturnType = 0;
    }
    
    if (ftype == NORMAL && funcReturnType == 0)
    {
	errorStream << "return type of \"";
	printInstanceDecl(errorStream, NORMAL_DECL, FALSE, FALSE, TRUE);
	errorStream << " defaulting to 'int'";
	caution(errorStream);
	funcReturnType = new IntType();
    }

    if (funcReturnType)
    {
	String tName = funcReturnType->typeName(NORMAL_DECL);

	funcReturnType->freezeType();
	
 	if ((tName != "void") && (tName != "const void"))
	{
	    hasNonVoidResult = TRUE;
	}
    }

    /* freeze argument and exception contexts */

    freezeContext(argList, argInfo);
    freezeContext(exceptionSpec, exceptionInfo);
    
    /* Set up stub specific options */

    if (funcClientVisibility == STUB_PENDING)
    {
	if (funcIsmemberOf == 0)
	    funcClientVisibility = STUB_NOREMOTE;
	else
	{
	    funcClientVisibility = funcIsmemberOf->defaultLocation();
	}
    }

    /* Check out parameter etc. capabilities */

    if (!funcPure &&
	hasNonVoidResult &&
	(funcClientVisibility == STUB_REMOTE))
    {
	if (funcReturnType->isTransferable())
	{
	    if (!funcReturnType->isFundamental())
	    {
		errorStream << "reference/pointer return value of \"";
		printInstanceDecl(errorStream, NORMAL_DECL,FALSE,FALSE,TRUE);
		errorStream << "\" may leak memory";
		caution(errorStream);
	    }
	}
	else
	{
	    remotelyCallable = FALSE;
	    errorStream << "return type of \"";
	    printInstanceDecl(errorStream, NORMAL_DECL, FALSE, FALSE, TRUE);
	    errorStream << "\" cannot be marshalled\n";
	    error(errorStream);
	}
    }

    /* checkout arglist marshalling */

    hasResultArgs = checkTransfer(argList, "argument ");
    checkTransfer(exceptionSpec, "exception ");

    if (ftype == CONVERSION)
    {
	remotelyCallable = FALSE;
	if (funcClientVisibility == STUB_REMOTE)
	{
	    errorStream << "stub code production for conversion operator \"";
	    printInstanceDecl(errorStream, NORMAL_DECL, FALSE, FALSE, TRUE);
	    errorStream << "\" not supported";
	
	    error(errorStream);
	}
    }

    if ((funcClientVisibility == STUB_REMOTE) && !remotelyCallable)
    {
	if (funcIsmemberOf != 0)
	   funcClientVisibility = STUB_NOREMOTE;
	else
	   funcClientVisibility = STUB_NOREMOTE;
    }

    if ((exceptionSpec == 0) &&
	(GlobalInfo::getOutputStyle() == CPLUSPLUS_STYLE) &&
	((funcIsmemberOf != 0) && funcIsmemberOf->membersUseExceptions()) &&
	remotelyCallable)
    {	
	errorStream << "\"";
	printInstanceDecl(errorStream, NORMAL_DECL, FALSE,FALSE,TRUE);
	errorStream << "\" may throw an exception that will not be propagated back to the client";
	warning(errorStream);
    }    
    
    if (funcPure && !funcVirtual)
    {
	errorStream << "only virtual functions can be pure";
	error(errorStream);
	funcPure = FALSE;
    }

    funcHashval = hashIt(funcSignature);
    
    convStrm << funcServerName << (funcHashval) << ends;

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

/*
 * Allow several modifiers that are harmless
 */

Boolean FctType::overrideStubModifiers ( StubModifier newSm )
{
#ifdef SG_DEBUG
    debugStream << "FctType::overrideModifiers("  << (void *)this << ","
		<< newSm << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    switch (newSm)
    {
    case STUB_NOMARSHALL:
    case STUB_AUTOMARSHALL:
	return TRUE;
    default:
	return addStubModifier(newSm);
    }
    
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
 * postname handles printing of the actual argument list, function quals
 * and any exception specifications.
 */

String FctType::postName ( DeclStyle ds ) const
{
    char buffer[1024];
    ostrstream s(buffer,1024);
    
    switch (ds)
    {
    case CLIENT_DECL:
	printArgList(s, ds, TRUE, FALSE, FALSE, TRUE);
	break;
    case SERVER_DECL:
	break;
    case SERVERIMPLEMENTATION_DECL:
    case NORMAL_DECL:
    case SHADOWS_DECL:
	printArgList(s, ds, funcInline ? FALSE:TRUE, FALSE, FALSE, FALSE);
	break;
    case IDL_DECL:
	printArgList(s, ds, FALSE, TRUE, TRUE, FALSE);

    }

    if ((funcQuals) && (ds != IDL_DECL))
    {
	s << " " << funcQuals->preName(NORMAL_DECL)
	  << funcQuals->postName(NORMAL_DECL);
    }

    if (exceptionSpec)
    {
	if (ds == IDL_DECL)
	    s << " raises (";
	else
	    s << " throw (";
	
	s << *exceptionSpec << ")";
	
    }
    
    if ((ds != IDL_DECL) && funcPure)
	s << " = 0";

    if (funcReturnType)
	s << funcReturnType->postName(ds);

    s << ends;
    
    return buffer;
}

String FctType::preName ( DeclStyle ds, Boolean, Boolean ) const
{
    String s;

    if (ds != IDL_DECL)
    {
	if (ftype == DESTRUCTOR)
	    s = "~";
	else
	    if (ftype == CONVERSION)
		s = "operator ";
    }
    
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
    default:
	return funcName;
    }
}

StubModifier FctType::defaultLocation () const
{
#ifdef SG_DEBUG
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
#ifdef SG_DEBUG
    debugStream << "FctType::defaultMarshall(" << (void * const)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return STUB_NOMARSHALL;
}

StubModifier FctType::defaultTransfer () const
{
#ifdef SG_DEBUG
    debugStream << "FctType::defaultTransfer(" << (void * const)this << ")\n";
    _debug.dbgFlush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    return STUB_ARG_NEITHER;
}
/*
 * We subsume isTransferable for functions not to mean the function
 * itself can be transfered but instead that its result type, arguments
 * and excpetions can be
 */

Boolean FctType::isTransferable () const
{
    return remotelyCallable;
}

Boolean FctType::produceMarshallingCode (ostream&, DeclStyle, Direction,
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
    case SHADOWS_DECL:
	printShadowsCode(s);
	break;
    default:
        break;
    }
    return TRUE;
}

ostream& FctType::printInstanceOf ( ostream& s,
				    const String&,
				    DeclStyle ds,
				    TypeDescriptor *qt,
				    Boolean b,
				    Boolean noConst)
{
    Boolean doQual = (qt ? TRUE : FALSE);
    ostrstream errorStream;
    
    switch (ds)
    {
    case SERVER_DECL:
	s << "Int32 " << funcServerName;
	s << "(" << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL) << " *, RpcBuffer&, RpcBuffer&)";
	break;
    case IDL_DECL:
	if (funcIsmemberOf)
	{
	    Boolean printed = FALSE;
	    
	    if (ftype == NORMAL)
	    {
		Declaration *d = funcIsmemberOf->lookFor(funcName, FUNCTION_TYPE,CURRENT_CONTEXT);

		if (d && d->getType() == this)
		{
		    printInstanceDecl(s,ds,b,noConst,FALSE);
		    printed = TRUE;
		}
	    }
	    
	    if (!printed)
	    {
		VoidType vt;
		
		String tName = funcName;
		funcName = funcServerName;
		if (ftype == CONSTRUCTOR || ftype == DESTRUCTOR)
		    funcReturnType = &vt;
		
		printInstanceDecl(s,ds,b,noConst,doQual);
		funcName = tName;
		errorStream << "OMG IDL does not support ";
		switch (ftype)
		{
		case CONSTRUCTOR:
		    errorStream << "constructors, ";
		    break;
		case DESTRUCTOR:
		    errorStream << "destructors, ";
		    break;
		case CONVERSION:
		    errorStream << "conversion, ";
		case OPER:
		    errorStream << "operators, ";
		    break;
		default:
		    errorStream << "function overloading, ";
		}
		
		errorStream << "changing signature of operation:\"";
		printInstanceDecl(errorStream ,NORMAL_DECL,b,noConst,doQual);
		errorStream << "\"";
		warning(errorStream);
		
		if (ftype == CONSTRUCTOR || ftype == DESTRUCTOR)
		    funcReturnType = 0;
	    }
	}
	else 
	    printInstanceDecl(s,ds,b,noConst,doQual);
        break;
    case CLIENT_DECL:
    case SHADOWS_DECL:
    case NORMAL_DECL:
    case SERVERIMPLEMENTATION_DECL:
	printInstanceDecl(s,ds,b,noConst,doQual);
    }

    return s;
}

Boolean FctType::checkTransfer ( DeclContext *dclcon, const char *msg )
{
    Declaration *decl = 0;
    int declIndex = 0;
    Boolean resultsReturned = FALSE;

    if (dclcon)
	while ((decl = dclcon->getOneMember(declIndex++)) != 0)
	{
	    TypeDescriptor *td = decl->getType();	
	    Boolean canMarshall = TRUE;

	    switch(decl->getTransfer())
	    {
	    case STUB_ARG_INOUT:
	    case STUB_ARG_OUT:
		resultsReturned= TRUE;	/* BEWARE deliberate fall through */
	    case STUB_ARG_IN:
		canMarshall = td->isTransferable();
		break;
	    default:
		canMarshall = FALSE;
		break;
	    }

	    if ((funcClientVisibility == STUB_REMOTE) && !canMarshall)
	    {
		ostrstream errorStream;

	        remotelyCallable = FALSE;
		errorStream << msg << declIndex << " of \"";
		printInstanceDecl(errorStream, NORMAL_DECL, FALSE,FALSE,TRUE);
		errorStream << "\" cannot be marshalled\n";
		error(errorStream);
	    }
	}

    return resultsReturned;
}

/*
 * Warning. Ordering of functions calls here is tricky due to when
 * things are set up by complete/freezeDecl.
 */

void FctType::setArgList ( DeclarationList *d )
{
#ifdef SG_DEBUG
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
	    while ((decl = next()))
	    {
		String name = decl->getName();
		TypeDescriptor *typ = decl->getType();

		if ((name == NullString))
		    decl->setName(constructName("Par_", parnum++));

		if (name == "...")
		    hasVarArgs = TRUE;

		/* Add to arg list NOW */

		argList->putIn(decl);

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

    if (exceptionSpec || hasNonVoidResult)
	s << "RpcBuffer _rvBuffer_;\n";
    
    s << "RpcBuffer _callBuffer_(_myHashVal);\n";
    s << "RpcBuffer _replyBuffer_;\n";
    s << "RPC_Status _rpcStatus_ = OPER_UNKNOWN;\n";
    s << "Int32 _serverStatus_ = OPER_INVOKED_OK;\n\n";

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

	    type->printInstanceOf(s, NullString, CLIENT_DECL, 0, FALSE, TRUE);

	    if (useNew)
	    {
		s << "& _returnedValue_ = * new ";
		s << type->typeName(CLIENT_DECL);
	    }
	    else
		s << "_returnedValue_";

	     s << type->defaultInit();

	    s << ";\n";
	}
}

void FctType::doCallToServer ( ostream& s )
{
    s << "_rpcStatus_ = _clientManager.call(" << funcHashval;
    s << ", _callBuffer_, _serverStatus_, _replyBuffer_);\n";
}

/*
 * Freeze arg/exception context. Build up param info for vector etc handling
 */

void FctType::freezeContext ( DeclContext *dclcon, ParamInfo *& info )
{
    Declaration *decl = 0;
    int declIndex = 0;

    if (dclcon)
    {
	int count = dclcon->noOfEntries();

	if (count > 0)
	{
	    info = new ParamInfo[count];
	    
	    while ((decl = dclcon->getOneMember(declIndex)))
	    {
		TypeDescriptor *t1, *t2, *t3;
		Type akind = NONE_TYPE;
		
		decl->freezeDecl();	/* ensures getType will work! */
		
		t1 = decl->getType();
		t2 = t1->simplify(CV_TYPE|TYPE_TYPE);
		akind = t2->kind();
		info[declIndex].serverDeclType = t2;
		info[declIndex].serverMarshallType = t2;
		info[declIndex].clientMarshallType = t1;
		
		if (akind == VECTOR_TYPE)
		{
		    t3 = t2->deref();
		    
		    info[declIndex].serverCleanUp = TRUE;
		    info[declIndex].arrayDelete = TRUE;
		    
		    info[declIndex].clientMarshallType = new PointerType(REFERENCE_TYPE,t2);
		    info[declIndex].serverDeclType = new PointerType(POINTER_TYPE,t3);
		    info[declIndex].serverMarshallType = new PointerType(REFERENCE_TYPE,t2);
		}
		else
		    if ((akind == POINTER_TYPE)|| (akind == REFERENCE_TYPE))
		    {
			info[declIndex].serverCleanUp = TRUE;
			
			/* check for pointer/ref to vector */
		    
			t3 = t2->deref()->simplify(CV_TYPE|TYPE_TYPE);
			
			if (t3->kind() == VECTOR_TYPE)
			{
			    info[declIndex].arrayDelete = TRUE;
			    info[declIndex].serverDeclType = new PointerType(POINTER_TYPE,t3->deref());
			    
			    if (akind == REFERENCE_TYPE)
			    {
				info[declIndex].cast = TRUE;
				info[declIndex].deref = TRUE;
				info[declIndex].serverMarshallType = t2;
			    }
			    else
			    {
				info[declIndex].serverMarshallType = new PointerType(REFERENCE_TYPE,t2->deref());
				info[declIndex].cast = TRUE;
				akind = REFERENCE_TYPE;
			    }
			}
			else
			    if (akind == REFERENCE_TYPE)
			    {
				/* Treat references to fundamental types as pointers in server! */
				Type tkind = NONE_TYPE;
				TypeDescriptor *t = t2->deref();
				
				t = t->simplify(CV_TYPE|TYPE_TYPE);
				tkind = t->kind();
				
				/* references to chars are also special! */
				
				if (tkind == CHAR_TYPE)
				{
				    info[declIndex].deref = TRUE;
				    info[declIndex].serverDeclType = new PointerType(POINTER_TYPE, t);
				    info[declIndex].serverMarshallType = new PointerType(REFERENCE_TYPE, t2);
				}
				else
				{
				    if (tkind == POINTER_TYPE)
				    {
					/* ref to pointer - discard ref in server */
					info[declIndex].serverDeclType = t;
				    }
				    else
					if (t->isFundamental())
					{
					    info[declIndex].deref = TRUE;
					    info[declIndex].clientMarshallType = new PointerType(POINTER_TYPE,t2);
					    info[declIndex].serverDeclType = new PointerType(POINTER_TYPE, t);
					    info[declIndex].serverMarshallType = new PointerType(POINTER_TYPE, t2);
					}
				}
			    }
		    }
		declIndex++;
	    }
	}
    }
}

void FctType::printArgList ( ostream& s, DeclStyle ds, Boolean noName, 
			     Boolean noDefault, Boolean noConst,
			     Boolean special ) const
{
    s << " ( ";
    
    if (argList && (argList->noOfEntries() != 0))
    {
	argList->printOn(s,ds,noName,noDefault,noConst);
    }
    
    if (ftype == CONSTRUCTOR && special)
    {
	if (argList->noOfEntries() != 0)
	    s << ", ";
	s << "ClientRpcManager *_cRpcMan_ ";
	if (!noDefault)
	{
	    String sName = GlobalInfo::getServerName();

	    if (sName == NullString)
		sName = funcServerName;

	    s << "= ClientRpcManager::createInstance(\"";	    
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

    while ((decl = argList->getOneMember(parnum)) != 0)
    {
	declSm = decl->getTransfer();

        if ((declSm == STUB_ARG_IN) || (declSm == STUB_ARG_INOUT))
        {
            if (!argInfo[parnum].clientMarshallType->produceMarshallingCode(s, CLIENT_DECL, IN, decl->getName(),"_callBuffer_"))
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

    while ((decl = argList->getOneMember(parnum)) != 0) /* iterate over arglist */
    {
	declSm = decl->getTransfer();
        if ((declSm == STUB_ARG_OUT) || (declSm == STUB_ARG_INOUT))
	{
            argInfo[parnum].serverMarshallType->produceMarshallingCode(s, SERVERIMPLEMENTATION_DECL,
								       IN, decl->getName(),"_result_");
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
    Boolean wasVirtual = funcVirtual;

    /* If function is pure don't want a definition */

    if (funcPure)
	return TRUE;

    /* Check that we can produce stub code for this operation */

    if (!remotelyCallable)
        return FALSE;

    /* OK - First produce the function header */

    s << "\n";

    funcVirtual = FALSE;		/* temporarily remove status */

    if (funcReturnType)
    {
	DeclContext *context = funcReturnType->getContext();
	
	if (context != 0)
	    s << context->getContextName(CLIENT_DECL);

	s << funcReturnType->preName(CLIENT_DECL) << " ";
    }

    funcVirtual = wasVirtual;		/* restore virtual status */
    s << className << "::";		/* class :: */

    switch (ftype)
    {
    case DESTRUCTOR:
        s << "~";			/* NOTE fall through */
    case CONSTRUCTOR:
	s << funcIsmemberOf->simpleTypeName(CLIENT_DECL);
	break;
    default:
	s << funcClientName;
    }

    printArgList(s, CLIENT_DECL, FALSE, TRUE, FALSE, TRUE);	/* augmented arg list */

    if (funcQuals)
    {
	s << " " << funcQuals->typeName(CLIENT_DECL);
    }

    if (exceptionSpec)
    {
	s << " throw (" << *exceptionSpec << ")";
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
	    
	    s << cl->typeName(CLIENT_DECL) << "(*_cRpcMan_, RpcBuffer())";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* Now non-virtual bases */

	blist = funcIsmemberOf->getNVBaseList();

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << cl->typeName(CLIENT_DECL) << "(*_cRpcMan_, RpcBuffer())";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* finally class members */

	s << "_clientManager (*_cRpcMan_), _myHashVal(-1)";
    }


    /* Now produce the function body */

    s << "\n{\n";

    /* First the declarations */

    declareClientLocals(s);
    
    if (ftype == CONSTRUCTOR)
	s << "_cRpcMan_->destroy();\n\n";    

    if (argList->noOfEntries() != 0)     /* pack up any args */
    {
        funcStubOk = packCallArgList(s);
    }

    s << "\n";

    doCallToServer(s);

    /* General function processing. Unpack any return args and the */
    /* return type/exception if it exists */

    s << "if (_rpcStatus_ == OPER_DONE)\n";
    s << "{\n";

    /* unpack result args first */
    
    if (hasResultArgs)
	unpackArgList(s);

    /* If oper was ok may have return value next */
    
    s << "switch(_serverStatus_)\n{\n";
    s << "case OPER_INVOKED_OK:\n";

    if (hasNonVoidResult)
    {
	s << "_replyBuffer_ >> _rvBuffer_;\n";
	funcFakeReturn->produceMarshallingCode(s, CLIENT_DECL, OUT, "_returnedValue_", "_rvBuffer_");
    }
    s << "break;\n";

    /* otherwise could have an exception value */
    /* exceptions encoded as an int (says which exception) and a value */
    if (exceptionSpec)
    {
	int exceptionCount = exceptionSpec->noOfEntries();
	if (exceptionCount != 0)
	{
	    IntType tempInt;
	
	    s << "case EXCEPTION_RAISED:\n{\n";
	    s << "int _exceptionNumber_ = 0;\n";
	    s << "_replyBuffer_ >> _rvBuffer_;\n";
	    tempInt.produceMarshallingCode(s, CLIENT_DECL,OUT, "_exceptionNumber_", "_rvBuffer_");
	    s << "switch (_exceptionNumber_)\n{\n";
    
	    /* Produce the exception cases */

	    for (int i = 0; i < exceptionCount; i++)
	    {
		Declaration *eDecl = exceptionSpec->getOneMember(i);
		TypeDescriptor *dummytype = 0;
		Type argkind;

		s << "case " << i << ":\n{\n";
		
		dummytype = eDecl->getType();
		argkind = dummytype->kind();

		if (argkind == TYPE_TYPE)
		    argkind = dummytype->deref()->kind();
		
		/* references to fundamental types get sent as pointers! */

		if (argkind == REFERENCE_TYPE)
		{
		    TypeDescriptor *t = dummytype->deref();

		    if (t->isFundamental())
			dummytype = new PointerType(POINTER_TYPE, t);
		}
	    
		dummytype->printInstanceOf(s, "_eType_", SERVERIMPLEMENTATION_DECL, 0, FALSE,TRUE);
		s << dummytype->defaultInit();
	    
		s << ";\n";
	    
		dummytype->produceMarshallingCode(s, CLIENT_DECL, OUT, "_eType_", "_rvBuffer_");
		s << "throw ";
		if (argkind == REFERENCE_TYPE)
		    s << "*";
		s << "_eType_;\n}\n";
	    }
	}
	s << "}\n";
	
	s << "break;\n}\n";
    }
    else
	if (funcIsmemberOf->membersUseExceptions())
	{
	    /* some other oper uses exceptions but this func has no
	       signature => it can throw anything which cannot be handled */

	    s << "case UNEXPECTED_EXCEPTION:\n";
	    s << "_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);\nbreak;\n";
	}
    
    s << "default:\n_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);\n";
    s << "}\n";
    
    s << "}\nelse\n_clientManager.rpcAbort(_rpcStatus_, _serverStatus_);\n\n";

    /* If op has return value, return it now */

    if (hasNonVoidResult)
    {
        Type returnkind;
        int bracelevel = 1;

        type = funcReturnType;
        s << "return (";
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
        s << "_returnedValue_";
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
    if (ds != IDL_DECL)
    {
	if (funcInline && ds != CLIENT_DECL)
	    s << "inline ";
	if (funcVirtual)
	    s << "virtual ";
    }
    
    if (funcReturnType != 0 && ftype != CONVERSION)
    {
	s << funcReturnType->preName(ds,elab,noConst);
	s << " ";
    }
    
    if (qualify && (funcIsmemberOf != 0))
	s << funcIsmemberOf->typeName(NORMAL_DECL, FALSE, TRUE) << "::";

    s << preName(ds,noConst);
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
 * as such here. References to pointers do not need such special treatment 
 * and so get left alone
 */

Boolean FctType::printServerCode ( ostream& s )
{
    Boolean cleanup = FALSE;
    int parnum = 0;
    Declaration *decl, *argdecl;
    Boolean handleExceptions = (exceptionSpec || funcIsmemberOf->membersUseExceptions() ? TRUE : FALSE);
    ostrstream errorStream;
      
    /* No code possible for pure functions */
    
    if (funcPure)
    {
	errorStream << "No stub code produced for pure function \"";
	printInstanceDecl(errorStream, NORMAL_DECL,FALSE,FALSE,TRUE);
	errorStream << "\"";
	warning(errorStream);
	return TRUE;
    }  
    
    if ((ftype == CONVERSION) || hasVarArgs)
        return FALSE;
    
    /* First produce the function header */
    
    s << "Int32 ";                       /* return type */
    s << funcIsmemberOf->typeName(SERVER_DECL) << "::"; /* class :: */
    s << funcServerName << " (";
    s << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL);
    s << " * _theObject_, RpcBuffer& _work_, RpcBuffer& _result_)";
    
    /* Now produce the function body */
    
    s << "\n{\n";

    if (handleExceptions || hasNonVoidResult)
	s << "RpcBuffer _rvBuffer_;\n";

    s << "Int32 _errCode_ = OPER_INVOKED_OK;\n";

    if (ftype == CONSTRUCTOR)
    {
	s << "int _constructedHere_ = 0;\n";
	s << "RpcBuffer _savedArgs_ = _work_;\n";
    }
    
    /* Produce the local variables for the parameters */
    
    if (argList->noOfEntries() != 0)
    {
	DeclContext *context = 0;
        String name;

	parnum = 0;

        while ((argdecl = argList->getOneMember(parnum)) != 0)
        {
            name = argdecl->getName();
	    context = argdecl->getType()->getContext();
		
	    if (context != 0)
		s << context->getContextName(SERVERIMPLEMENTATION_DECL);
		
	    argInfo[parnum].serverDeclType->printInstanceOf(s, name, SERVERIMPLEMENTATION_DECL,
							    0, FALSE, TRUE);
	    s << argInfo[parnum].serverDeclType->defaultInit();
		
	    s << ";\n";

	    argInfo[parnum].serverMarshallType->produceMarshallingCode(s, SERVERIMPLEMENTATION_DECL,
								       OUT, name, "_work_");

	    if (argInfo[parnum].serverCleanUp)
		cleanup = TRUE;
	    
	    parnum++;   
	}
	
	s << "if (_work_.readState() == BUFFER_GOOD)\n{";
    }
    s << "\n";
    
    if (handleExceptions)
    {
	s << "try\n{\n";
    }
    
    if (ftype == DESTRUCTOR)            /* real object destruction */
    {  
	s << "if (--_constructRefCount_ == 0)\n{\n";
        s << "delete _theObject_;\n";
        s << "_theObject_ = 0;\n";
	s << "theRealObject = 0;\n";
        s << "_constructOpcode_ = 0;\n";
	s << "}\n";
    } else
    {
        if (ftype == CONSTRUCTOR)       /* requires realobject construction */
        {
	    s << "if ((_constructRefCount_ > 0) && "
	      << "((_constructOpcode_ != " << funcHashval << ") || "
	      << "(_constructArgs_ != _savedArgs_)))\n";
	    
	    s << "_errCode_ = ALREADY_CONSTRUCTED;\n";
	    s << "else\n{\n";
	    s << "if (_constructRefCount_ > 0)\n";
	    s << "{\n_result_ = _constructResult_;\n";
	    s << "}\n";
	    s << "else\n{\n";
	    s << "_constructedHere_ = 1;\n";
            s << "theRealObject = new " << funcIsmemberOf->typeName(SERVERIMPLEMENTATION_DECL);
        } else
        {                               /* normal operation */
            if (hasNonVoidResult)
	    {
		DeclContext * context = funcReturnType->getContext();
		
		if (context != 0)
		    s << context->getContextName(SERVERIMPLEMENTATION_DECL);
	
                funcReturnType->printInstanceOf(s, "_returnedValue_", NORMAL_DECL, 0, FALSE);
		s << " = ";
	    }
	    
            s << "_theObject_->" << funcName;
        }
	
        s << "(";                       /* open parlist */
	
	parnum = 0;
        while ((decl = argList->getOneMember(parnum)))
        {
	    if (argInfo[parnum].cast)
		s << "(" << decl->getType()->typeName(NORMAL_DECL)
		  << ")";
	    
	    if (argInfo[parnum].deref)
		s << "*";
	    
	    if (argInfo[parnum].takeAddr)
		s << "&";
		    
            s << decl->getName();
	    parnum++;
	    
            if (parnum < argList->noOfEntries())
                s << ", ";
        }
        s << ");";
    }
    s << "\n";
    
    /* Call has been made, see if any arguments and/or a return value is */
    /* indicated  */

    if (hasNonVoidResult)
    { 
	funcReturnType->produceMarshallingCode(s, SERVERIMPLEMENTATION_DECL, IN, "_returnedValue_", "_rvBuffer_");

	if (freeResult)
	{
	    Type reskind = funcReturnType->kind();
	    
	    if (reskind == TYPE_TYPE)
	    {
		reskind = funcReturnType->deref()->kind();
	    }
	    if (reskind == POINTER_TYPE)
	    {
		s << "delete _returnedValue_;\n";
	    }
	    else
	    {
		errorStream << "Stub modifier '" << STUB_DELETE << "' "
			    << "not applicable in this declaration (ignored)";
		warning(errorStream); 
	    }
	}
	
    }

    if (ftype == CONSTRUCTOR)
	s << "}\n}\n";
    
    if (handleExceptions)
    {
	s << "}\n";
	
	if (exceptionSpec)
	{
	    int exceptionCount = exceptionSpec->noOfEntries();
	    if (exceptionCount != 0)
	    {
		/* Produce the exception cases */

		for (int i = 0; i < exceptionCount; i++)
		{
		    Declaration *eDecl = exceptionSpec->getOneMember(i);

		    s << "catch (";
		    eDecl->printDecl(s, SERVERIMPLEMENTATION_DECL, "");
		    s << ")\n{\n";
		    s << "_rvBuffer_ << " << i << ";\n";
	    
		    eDecl->produceMarshallCode(s, SERVERIMPLEMENTATION_DECL, IN, "_rvBuffer_");
		    s << "_errCode_ = EXCEPTION_RAISED;\n";
		    s << "}\n";    
		}
	    }
	}
	s << "catch(...)\n{\n";
	s << "_errCode_ = UNEXPECTED_EXCEPTION;\n";
	s << "}\n";
    }
       
    if (hasResultArgs)
    {
	if (ftype == CONSTRUCTOR)
	    s << "if (_constructedHere_)\n{\n";
	
	packResultArgList(s);

	if (ftype == CONSTRUCTOR)
	    s << "}\n";
    }
    
    if (handleExceptions || hasNonVoidResult)
    {
	s << "if (_rvBuffer_.length() > 0)\n";
	s << "_result_ << _rvBuffer_;\n";
    }
	
    if (ftype == CONSTRUCTOR)
    {
	s << "if (_errCode_ == OPER_INVOKED_OK)\n{\n";
	s << "_constructRefCount_++;\n";
	s << "if (_constructRefCount_ == 1)\n{\n";
	s << "_constructResult_ = _result_;\n";
	s << "_constructArgs_ = _savedArgs_;\n";
	s << "_constructOpcode_ = " << funcHashval << ";\n";
	s << "}\n}\n";
    }

    if (argList->noOfEntries() != 0)
	s << "}\nelse\n_errCode_ = PARAMETER_ERROR;\n";
    
    /* Cleanup up any allocated variables */

    if (cleanup)
    {
        String name;
	TypeDescriptor *dummytype = 0;

	parnum = 0;
	s << "\n_work_.rewrite();\n\n";
	
        while ((argdecl = argList->getOneMember(parnum)) != 0)
        {
            dummytype = argInfo[parnum].serverDeclType;
            name = argdecl->getName();
	    
            if (argInfo[parnum].serverCleanUp)
            {
		if (dummytype->deleteable())
		{
		    s << "if(_work_.shouldPack(" << name << "))\n";
		    s << "delete ";
		    if (argInfo[parnum].arrayDelete)
			s << "[] ";
		    
		    s<< name << ";\n";
		}
	    }
	    
	    parnum++;
        }
    }
    
    s << "return _errCode_;\n";
    s << "}\n\n";
    return TRUE;
}

Boolean FctType::printShadowsCode ( ostream& s )
{
    Boolean funcStubOk = TRUE;
    String className = funcIsmemberOf->typeName(SHADOWS_DECL);
    Boolean wasVirtual = funcVirtual;
    int parnum;
    Declaration *decl;

    /* If function is pure don't want a definition */

    if (funcPure)
	return TRUE;

    /* Check that we can produce stub code for this operation */

    if (!remotelyCallable)
        return FALSE;

    /* OK - First produce the function header */

    s << "\n";

    funcVirtual = FALSE;		/* temporarily remove status */

    if (funcReturnType)
    {
	s << funcReturnType->preName(SHADOWS_DECL) << " ";
    }

    funcVirtual = wasVirtual;		/* restore virtual status */
    s << className << "::";		/* class :: */

    switch (ftype)
    {
    case DESTRUCTOR:
        s << "~";			/* NOTE fall through */
    case CONSTRUCTOR:
	s << funcIsmemberOf->simpleTypeName(SHADOWS_DECL);
	break;
    default:
	s << funcName;
    }

    printArgList(s, SHADOWS_DECL, FALSE, TRUE, FALSE, FALSE);

    if (funcQuals)
    {
	s << " " << funcQuals->typeName(SHADOWS_DECL);
    }

    if (exceptionSpec)
    {
	s << " throw (" << *exceptionSpec << ")";
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
	    
	    s << cl->typeName(SHADOWS_DECL) << "(_local, _remote)";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* Now non-virtual bases */

	blist = funcIsmemberOf->getNVBaseList();

	while (blist)
	{
	    ClassType *cl = blist->getClass();
	    
	    s << cl->typeName(SHADOWS_DECL) << "(_local, _remote)";
	    blist = blist->getNext();
	    s << ", ";
	}

	/* finally class members */

	s << "_local (0)";
	s << ", _remote(0)";

	/* Now produce the function body */

	s << "\n{\n";
	s << "_local = new " << funcImplName;
    
	s << "(";                       /* open parlist */
	
	parnum = 0;
	while ((decl = argList->getOneMember(parnum++)))
	{
	    s << decl->getName();
	    if (parnum < argList->noOfEntries())
		s << ", ";
	}
	s << ");";
	s << "\n}\n";
    }
    else
	if (ftype == DESTRUCTOR)
	{
	    s << "\n{}\n";
	    
	}
	else
	{
	    /* General function processing. */

	    s << "\n{\n";
	    
	    if (hasNonVoidResult)
	    {
		s << "return ";
	    }

	    s << "}\n";
	}
    
    return funcStubOk;

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

    while ((decl = argList->getOneMember(parnum)) != 0)
    {
	declSm = decl->getTransfer();
        if ((declSm == STUB_ARG_OUT) ||
            (declSm == STUB_ARG_INOUT))
        {
	    argInfo[parnum].clientMarshallType->produceMarshallingCode(s, CLIENT_DECL,
								       OUT, decl->getName(),"_replyBuffer_");

        }
        parnum++;
    }
}

#ifdef NO_INLINES
#  define FUNCT_CC_
#  include "FuncT.n"
#  undef FUNCT_CC_
#endif
