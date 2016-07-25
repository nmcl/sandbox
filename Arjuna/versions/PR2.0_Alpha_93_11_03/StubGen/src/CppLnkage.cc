/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CppLnkage.cc,v 1.1 1993/11/03 14:47:17 nmcl Exp $
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

static const char RCSid[] = "$Id: CppLnkage.cc,v 1.1 1993/11/03 14:47:17 nmcl Exp $";

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
