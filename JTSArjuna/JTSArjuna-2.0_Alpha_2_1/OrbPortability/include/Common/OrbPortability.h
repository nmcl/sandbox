/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */

#ifndef COMMON_ORBPORTABILITY_H_
#define COMMON_ORBPORTABILITY_H_

/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */

#ifdef __STDC__
#  include <Common/OrbPortability_stdc.h>
#else
#  include <Common/OrbPortability_krc.h>
#endif

#endif
