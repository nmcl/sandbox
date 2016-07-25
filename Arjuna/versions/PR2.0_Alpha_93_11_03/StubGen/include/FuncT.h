/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncT.h,v 1.1 1993/11/03 14:45:11 nmcl Exp $
 */

#ifndef FUNCT_H_
#define FUNCT_H_

/*
 *
 * Stub generator function definition handler. Holds all information
 * about any particular function.
 *
 */

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef DCLCONTXT_H_
#  include "DclContxt.h"
#endif

#ifndef ADCLCNTXT_H_
#  include "ADclCntxt.h"
#endif

enum FunctionType { CONSTRUCTOR = 1, DESTRUCTOR, CONVERSION, OPER, NORMAL };

class ClassType;
class DeclarationList;

class FctType : public TypeDescriptor
{
public:
    FctType (ClassType * = 0, DeclarationList * = 0, TypeDescriptor * = 0,
	     FunctionType = NORMAL);

    ~FctType ();

    /* non-virtual functions */

    /* Function building */

    void makeConversion ();
    void makePure ();

    void setReturnType (TypeDescriptor *);

    /* Function interrogation */

    long getFuncHashval () const;
    TypeDescriptor *getReturnType () const;
    Boolean isPure () const;

    /* virtual functions inherited from base class */

    /* type building */

    virtual Boolean addBody (Expression *);
    virtual Boolean addFuncModifier (FuncModifier);
    virtual Boolean addName(const String&);
    virtual Boolean addStubModifier (StubModifier);

    virtual Boolean attachBaseType (TypeDescriptor *);
    virtual TypeDescriptor *combineTypes (TypeDescriptor *);
    
    virtual Boolean freezeType ();

    /* type interrogation */

    virtual Boolean isFundamental () const;
    virtual String massageName (const String&, DeclStyle) const;

    virtual String postName (DeclStyle) const;
    virtual String preName (DeclStyle = NORMAL_DECL, Boolean = FALSE,
			    Boolean = FALSE) const;

    virtual String signature () const;
    virtual String simpleTypeName (DeclStyle) const;
    /* stub generation */

    virtual StubModifier defaultLocation () const;
    virtual StubModifier defaultMarshall () const;
    virtual StubModifier defaultTransfer () const;
    
    virtual Boolean isTransferable () const;

    virtual Boolean produceMarshallingCode (ostream&, Direction,
                                            const String&, const String&);

    virtual Boolean produceStubCode (ostream&, DeclStyle);

    /* type printing */

    virtual ostream& printInstanceOf (ostream&, const String&, DeclStyle,
                                      Boolean, Boolean = FALSE);

private:
    void setArgList (DeclarationList *);

    void declareClientLocals (ostream&);
    void doCallToServer(ostream&);

    void printArgList (ostream&, DeclStyle, Boolean, Boolean, Boolean) const;

    Boolean packCallArgList(ostream&);
    Boolean packResultArgList (ostream&);

    Boolean printClientCode(ostream&);
    void printInstanceDecl (ostream&, DeclStyle, Boolean, Boolean, Boolean);
    Boolean printServerCode(ostream&);
    void unpackArgList (ostream&);

    ArgDeclContext argList;		/* arguments for this function */
    DeclarationList *argsPending;	/* pending args */
    FunctionType ftype;
    Expression *funcBody;		/* body of func */
    String funcClientName;
    StubModifier funcClientVisibility;	/* remote or not */

    Boolean funcFrozen;
    long funcHashval;
    String funcImplName;
    Boolean funcInline;
    ClassType *funcIsmemberOf;		/* aggregate fn is member of */
    String funcName;			/* function name */
    Boolean funcPure;
    TypeDescriptor *funcQuals;
    String funcServerName;
    TypeDescriptor *funcReturnType;	/* return type (if any) */
    TypeDescriptor *funcFakeReturn;
    String funcSignature;		/* mangled name */
    Boolean funcVirtual;
    

    Boolean hasResultArgs;
    Boolean hasVarArgs;
    Boolean hasNonVoidResult;		/* return type is not void */
};

#include "FuncT.n"

#endif
