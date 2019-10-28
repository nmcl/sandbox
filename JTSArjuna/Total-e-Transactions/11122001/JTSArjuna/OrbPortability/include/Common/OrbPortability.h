/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1.88.1 2001/08/09 16:15:43 delsworthy Exp $
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
