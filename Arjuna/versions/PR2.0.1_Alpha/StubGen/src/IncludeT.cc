/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IncludeT.cc,v 1.1 1993/11/03 14:47:53 nmcl Exp $
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

static const char RCSid[] = "$Id: IncludeT.cc,v 1.1 1993/11/03 14:47:53 nmcl Exp $";

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
#ifdef DEBUG
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
#ifdef DEBUG
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

String IncludeType::massageName ( const String&, DeclStyle ) const
{
#ifdef SHORTNAMES
    String ftail("_s.h");
#else
    String ftail("_stub.h");
#endif
    
    int tlen = ftail.size();
    String nameTail(includeFname(includeFname.size()-tlen));

    
    if (noMangle || (nameTail == ftail))
	return includeFname;
    else
	return includeFname(0,includeFname.size()-2) + ftail;
    
}

/*
 * Note we force a newline to ensure that the # character is in col 1
 */

ostream& IncludeType::printInstanceOf ( ostream& s, const String& name,
					DeclStyle, Boolean, Boolean )
{
    s << "\n#include ";

    if (isSys)
	s << "<";
    else
	s << "\"";

    s << massageName(name, NORMAL_DECL);
    
    if (isSys)
	s << ">";
    else
	s << "\"";

    s << "\n";

    return s;
}

