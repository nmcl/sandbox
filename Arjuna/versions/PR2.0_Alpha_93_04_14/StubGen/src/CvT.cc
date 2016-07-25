/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CvT.cc,v 1.13 1993/04/14 13:46:04 ngdp Exp $
 */

/*
 * Stub generator const/volatile type definition handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#  include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef CVT_H_
#  include "CvT.h"
#endif

#ifndef MODIFIEDT_H_
#  include "ModifiedT.h"
#endif

#ifndef INTT_H_
#  include "IntT.h"
#endif

static const char RCSid[] = "$Id: CvT.cc,v 1.13 1993/04/14 13:46:04 ngdp Exp $";

/*
 * PUBLIC operations
 */

CvType::CvType ( CvQualifier cvt )
               : TypeDescriptor(CV_TYPE),
		 cv_type(0),
		 is_const(FALSE),
		 is_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "CvType::CvType(" << (void *)this << ","
	         << cvt << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    add_cv_qualifier(cvt);
}

CvType::CvType ( TypeDescriptor *cvt )
               : TypeDescriptor(CV_TYPE),
		 cv_type(cvt),
		 is_const(FALSE),
		 is_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "CvType::CvType(" << (void *)this << ","
	         << (void*)cvt << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

CvType::CvType ()
               : TypeDescriptor(CV_TYPE),
		 cv_type(0),
		 is_const(FALSE),
		 is_volatile(FALSE)
{
#ifdef DEBUG
    debug_stream << "CvType::CvType\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void CvType::add_type_modifier ( TypeModifier newmod )
{
#ifdef DEBUG
    debug_stream << "CvType::add_type_modifier (" << (void *)this << "," 
	         << newmod << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cv_type == 0)
    {
	cv_type = new ModifiedType();
    }

    cv_type->add_type_modifier(newmod);
}

CvType::~CvType ()
{
#ifdef DEBUG
    debug_stream << "CvType::~CvType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void CvType::add_cv_qualifier ( CvQualifier new_tq )
{
#ifdef DEBUG
    debug_stream << "CvType::add_cv_qualifier (" << (void *)this << ","
	         << new_tq << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
    
    if (new_tq == QUAL_CONST)
    {
        if (is_const)
            warning("superfluous 'const' ignored");
	else if (is_volatile)
	    error("both 'const' and 'volatile' specified in this declaration");
        is_const = TRUE;
    }
    else
    {
        if (is_volatile)
            warning("superfluous 'volatile' ignored");
	else if (is_const)
	    error("both 'const' and 'volatile' specified in this declaration");
        is_volatile = TRUE;
    }

}

void CvType::add_stub_modifier ( StubModifier sm )
{
    if ((sm == STUB_ARG_OUT) || (sm == STUB_ARG_INOUT))
    {
	error_stream << "stub command '" << sm 
	             << "' illegal in this declaration";
	error(error_stream);
    }
}

/*
 * Merging  const & volatile types is different. Rather than merge
 * the const etc attributes into the indicated type, we merely point
 * to it.
 */

TypeDescriptor *CvType::combine_types ( TypeDescriptor *newtype )
{
#ifdef DEBUG
    debug_stream << "CvType::combine_types ("  << (void *)this << ","
	         << (void *)newtype << ")\n";
    _debug.dbg_flush(FUNCTIONS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    if (cv_type == 0)
    {
	cv_type = newtype;
    }
    else
	cv_type = cv_type->combine_types(newtype);

    return this;
}

StubModifier CvType::default_par_transfer () const
{
    if (is_const)
	return STUB_ARG_IN;
    else
	if (cv_type)
	    return cv_type->default_par_transfer();
	else
	    return STUB_ARG_IN;
}

TypeDescriptor *CvType::deref () const
{
    return cv_type;
}

void CvType::freeze_type ()
{
    if (cv_type == 0)
	cv_type = new IntType();

    cv_type->freeze_type();
}

Boolean CvType::is_constant () const
{
    return is_const;
}

Boolean CvType::is_fundamental () const
{
    if (cv_type)
	return cv_type->is_fundamental();
    else
	return TRUE;
}

Boolean CvType::is_transmissible () const
{
    if (cv_type)
	return cv_type->is_transmissible();
    else
	return FALSE;
}

void CvType::print_marshalling_code ( ostream& s,
					 const String& varname, 
					 const String& buffname) 
{
    s << "\t" << buffname << " << " ;
    s << varname << ";\n";
}

void CvType::print_unmarshalling_code ( ostream& s,
					   const String& varname, 
					   const String& buffname) 
{
    s << "\t" << buffname << " >> " ;
    s << varname << ";\n";
}

int CvType::priority () const
{
    return 1;
}

String CvType::signature () const
{
    String sig;

    if (is_const)
	sig += "C";
    if (is_volatile)
	sig += "V";

    if (cv_type)
	sig += cv_type->signature();

    return sig;    
}

String CvType::typename ( DeclStyle ds ) const
{
    String root;

    if (is_const)
	root += "const ";
    if (is_volatile)
	root += "volatile ";

    if (cv_type)
	root += cv_type->typename(ds);

    return root;
}

void CvType::postname ( ostream& s, DeclStyle ds ) 
{
    if (cv_type)
	cv_type->postname(s,ds);
}

void CvType::prename ( ostream& s, DeclStyle ds ) 
{
    if (cv_type)
	cv_type->prename(s,ds);
}

void CvType::print_bt ( ostream& s, DeclStyle ds, Boolean elaborated, Boolean ignoreConst ) 
{
    if (is_const && !ignoreConst)
	s << "const ";
    if (is_volatile)
	s << "volatile ";
    if (cv_type)
	cv_type->print_bt(s, ds, elaborated, ignoreConst);
}


