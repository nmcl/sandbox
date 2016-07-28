/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.cc,v 1.6 1995/03/14 09:11:44 ngdp Exp $
 */

/*
 * Stub generator include file handler
 *
 */

#include <strstream.h>

#ifdef SG_DEBUG
#   include "Debug.h"
#endif

#ifndef ERROR_H_
#  include "Error.h"
#endif

#ifndef INCLUDET_H_
#  include "IncludeT.h"
#endif

static const char RCSid[] = "$Id: IncludeT.cc,v 1.6 1995/03/14 09:11:44 ngdp Exp $";

/*
 * PUBLIC operations:
 */

IncludeType::IncludeType ( const String& dname, const String& fname,
			   StubModifier sm ) 
                         : DummyType(),
			   includeDir(dname),
			   includeFname(fname),
			   isSys(dname == NullString ? FALSE : TRUE),
			   noMangle(sm == STUB_NORENAME ? TRUE : FALSE)
{
#ifdef SG_DEBUG
    debugStream << "IncludeType::IncludeType(" << dname << "," ;
    debugStream << fname << ")\n";
    _debug.dbgFlush(CONSTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif

    String suf = fname.rindex('.');
    
    if (suf != ".h")
	noMangle = TRUE;   
}

IncludeType::~IncludeType ()
{
#ifdef SG_DEBUG
    debugStream << "IncludeType::~IncludeType(" << (void *)this << ")\n";
    _debug.dbgFlush(DESTRUCTORS, FAC_BASIC_TYPES, VIS_PUBLIC);
#endif
}

Boolean IncludeType::addStubModifier ( StubModifier sm )
{
    if (sm == STUB_NORENAME)
	noMangle = TRUE;
    else
	return DummyType::addStubModifier(sm);

    return TRUE;
}

String IncludeType::massageName ( const String&, DeclStyle ds ) const
{
    String ftail(ds == IDL_DECL ? ".idl" : "_stub.h");
    
    int tlen = ftail.size();
    String nameTail(includeFname(includeFname.size()-tlen,0));

    
    if ((ds != IDL_DECL) &&
	(noMangle || (nameTail == ftail)))
	return includeFname;
    else
	return includeFname(0,includeFname.size()-2) + ftail;
    
}

/*
 * Note we force a newline to ensure that the # character is in col 1
 */

ostream& IncludeType::printInstanceOf ( ostream& s, const String& name,
					DeclStyle ds, TypeDescriptor *,
					Boolean, Boolean )
{
    s << "\n#include ";

    if (isSys)
	s << "<";
    else
	s << "\"";

    s << massageName(name, ds);
    
    if (isSys)
	s << ">";
    else
	s << "\"";

    s << "\n";

    return s;
}

