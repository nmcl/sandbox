/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CppLnkage.cc,v 1.1.6.1 1996/10/10 12:26:49 ngdp Exp $
 */

/*
 * Stub generator C linkage routines
 *
 */

#ifndef WIN32
#  include <strstream.h>
#else
#  include <strstrea.h>
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
#endif

static const char RCSid[] = "$Id: CppLnkage.cc,v 1.1.6.1 1996/10/10 12:26:49 ngdp Exp $";

/*
 * Declare the only instance of this class we will ever need
 */

CppLinkage *cppLinkage = new CppLinkage;

String CppLinkage::mangle ( TypeDescriptor *td ) const
{
    return td->signature();
}

ostream& CppLinkage::printLinkage ( ostream &s )
{
    s << "extern \"C++\" ";

    return s;
}