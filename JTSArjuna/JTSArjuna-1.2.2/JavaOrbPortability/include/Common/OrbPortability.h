/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
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
