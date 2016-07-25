/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FuncT.h,v 1.12 1993/04/14 13:45:43 ngdp Exp $
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
    FctType (ClassType * = 0, DeclarationList * = 0,
	     FunctionType = NORMAL);

    ~FctType ();

    /* non-virtual functions */

    long get_func_hashval () const;
    String get_funcname () const;
    TypeDescriptor *get_return_type () const;
    String get_servername () const;

    void make_pure ();

    void print_arglist (ostream&, DeclStyle, Boolean, Boolean, Boolean);

    void set_arglist (DeclarationList *);
    void set_conversion ();
    void set_quals (TypeDescriptor *);
    void set_return_type (TypeDescriptor *);

    /* virtual functions */

    virtual void add_body (Expression *);
    virtual void add_func_modifier (FuncModifier);
    virtual void add_name (const String&);
    virtual void add_stub_modifier (StubModifier);
    virtual void attach_base_type (TypeDescriptor *);

    virtual TypeDescriptor *combine_types (TypeDescriptor *);

    virtual StubModifier default_location () const;
    virtual StubModifier default_marshall () const;
    virtual StubModifier default_par_transfer () const;

    virtual void freeze_type ();

    virtual Boolean is_fundamental () const;
    virtual Boolean is_pure () const;
    virtual Boolean is_transmissible () const;
    virtual String massage_name (const String&, DeclStyle) const;

    virtual void postname (ostream&, DeclStyle);
    virtual void prename (ostream&, DeclStyle);
    virtual void print_bt (ostream&, DeclStyle, Boolean, Boolean);
    virtual Boolean print_code (ostream&, DeclStyle);
    virtual void print_instance (ostream&, const String&, DeclStyle,
				 Boolean, Boolean);
    virtual void print_marshalling_code (ostream&, const String&,
                                         const String&);

    virtual void print_typedef (ostream&, DeclStyle);
    virtual void print_unmarshalling_code (ostream&, const String&,
                                         const String&);

    virtual String signature () const;
    virtual String typename (DeclStyle) const;

private:
    void declare_client_locals (ostream&);
    Boolean pack_call_arglist(ostream&);
    void pack_result_arglist (ostream&);
    void print_callto_server(ostream&);
    void unpack_arglist (ostream&);
    Boolean print_client_code(ostream&);
    Boolean print_server_code(ostream&);
    void print_client_instance (ostream&, const String&, Boolean);
    void print_server_instance (ostream&, const String&, Boolean);

    ArgDeclContext arglist;		/* arguments for this function */
    DeclarationList *args_pending;	/* pending args */
    FunctionType ftype;
    Expression *func_body;		/* body of func */
    String func_clientname;
    StubModifier func_clientvisibility;	/* remote or not */

    Boolean func_frozen;
    long func_hashval;
    String func_implname;
    Boolean func_inline;
    ClassType *func_ismemberof;		/* aggregate fn is member of */
    String func_name;			/* function name */
    Boolean func_pure;
    TypeDescriptor *func_quals;
    String func_servername;
    TypeDescriptor *func_return_type;	/* return type (if any) */
    TypeDescriptor *func_fake_return;
    String func_signature;		/* mangled name */
    Boolean func_virtual;
    
    Boolean has_varargs;
    Boolean has_result_args;

};

#include "FuncT.n"

#endif
