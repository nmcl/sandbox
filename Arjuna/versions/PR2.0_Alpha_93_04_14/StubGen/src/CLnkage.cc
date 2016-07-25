/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CLnkage.cc,v 1.5 1993/04/14 13:45:59 ngdp Exp $
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

#ifndef CLNKAGE_H_
#  include "CLnkage.h"
#endif

static const char RCSid[] = "$Id: CLnkage.cc,v 1.5 1993/04/14 13:45:59 ngdp Exp $";

/*
 * Declare the only instance of this class we will ever need
 */

CLinkage *c_linkage = new CLinkage;

String CLinkage::mangle ( TypeDescriptor *td ) const
{
    return td->typename(NORMAL_DECL);
}

ostream& CLinkage::print_linkage ( ostream& s )
{
    s << "extern \"C\" ";

    return s;
}
