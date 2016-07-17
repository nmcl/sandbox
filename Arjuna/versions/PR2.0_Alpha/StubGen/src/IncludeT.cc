/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.cc,v 1.9 1993/04/14 13:46:14 ngdp Exp $
 */

/*
 * Stub generator include file handler
 *
 */

#include <strstream.h>

#ifdef DEBUG
#   include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INCLUDET_H_
#  include "IncludeT.h"
#endif

static const char RCSid[] = "$Id: IncludeT.cc,v 1.9 1993/04/14 13:46:14 ngdp Exp $";

/*
 * PUBLIC operations:
 */

IncludeType::IncludeType ( const String& dname, const String& fname,
			   StubModifier sm ) 
                         : DummyType(),
			   include_dir(dname),
			   include_fname(fname),
			   is_sys(dname == NullString ? FALSE : TRUE),
			   no_mangle(sm == STUB_NORENAME ? TRUE : FALSE)
{
#ifdef DEBUG
    debug_stream << "IncludeType::IncludeType(" << dname << "," ;
    debug_stream << fname << ")\n";
    _debug.dbg_flush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    String suf = fname.rindex('.');
    
    if (suf != ".h")
	no_mangle = TRUE;   
}

IncludeType::~IncludeType ()
{
#ifdef DEBUG
    debug_stream << "IncludeType::~IncludeType(" << (void *)this << ")\n";
    _debug.dbg_flush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

void IncludeType::add_stub_modifier ( StubModifier sm )
{
    if (sm == STUB_NORENAME)
	no_mangle = TRUE;
    else
	DummyType::add_stub_modifier(sm);
}

String IncludeType::massage_name ( const String&, DeclStyle ) const
{
#ifdef SHORTNAMES
    String ftail("_s.h");
#else
    String ftail("_stub.h");
#endif
    
    int tlen = ftail.size();
    String nameTail(include_fname(include_fname.size()-tlen));

    
    if (no_mangle || (nameTail == ftail))
	return include_fname;
    else
	return include_fname(0,include_fname.size()-2) + ftail;
    
}

void IncludeType::prename ( ostream& s, DeclStyle )
{
    s << "#include ";
    if (is_sys)
	s << "<";
    else
	s << "\"";
}

void IncludeType::postname ( ostream& s, DeclStyle )
{
    if (is_sys)
	s << ">";
    else
	s << "\"";
    s <<"\n";
}

String IncludeType::typename ( DeclStyle ) const
{
    return include_fname;
}

String IncludeType::signature () const
{
    return include_fname;
}

