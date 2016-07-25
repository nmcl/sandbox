/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CLnkage.cc,v 1.6 1993/06/17 15:08:50 ngdp Exp $
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

static const char RCSid[] = "$Id: CLnkage.cc,v 1.6 1993/06/17 15:08:50 ngdp Exp $";

/*
 * Declare the only instance of this class we will ever need
 */

CLinkage *cLinkage = new CLinkage;

String CLinkage::mangle ( TypeDescriptor *td ) const
{
    return td->typeName(NORMAL_DECL);
}

ostream& CLinkage::printLinkage ( ostream& s )
{
    s << "extern \"C\" ";

    return s;
}
