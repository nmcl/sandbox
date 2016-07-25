/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CppLnkage.cc,v 1.4 1993/03/22 09:30:44 ngdp Exp $
 */

/*
 * Stub generator C linkage routines
 *
 */

#include <strstream.h>

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef TDESC_H_
#  include "TDesc.h"
#endif

#ifndef CPPLNKAGE_H_
#  include "CppLnkage.h"
#endif

static const char RCSid[] = "$Id: CppLnkage.cc,v 1.4 1993/03/22 09:30:44 ngdp Exp $";

/*
 * Declare the only instance of this class we will ever need
 */

CppLinkage *cpp_linkage = new CppLinkage;

String CppLinkage::mangle ( TypeDescriptor *td ) const
{
    return td->signature();
}

ostream& CppLinkage::print_linkage ( ostream &s )
{
    s << "extern \"C++\" ";

    return s;
}
