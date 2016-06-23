/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Boolean.cc,v 1.1 1997/09/25 15:25:25 nmcl Exp $
 */

/*
 *
 * Boolean class
 *
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

static const char RCSid[] = "$Id";

#ifndef HAVE_BOOLEAN_TYPE

const Boolean TRUE  = 1;
const Boolean FALSE = 0;

#endif
