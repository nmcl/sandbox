/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: dummy_H.h,v 1.1 1998/05/27 11:22:06 nmcl Exp $
 */

#if defined(ORBIX) || defined(OMNIBROKER) || defined(OMNIORB)
#  include "dummy.h"
#endif

#ifdef OMNIBROKER
#  include "dummy_skel.h"
#endif

#ifdef ORBPLUS
#  include "dummyTypes.h"
#  include "dummyServer.h"
#endif

#ifdef RCPORB
#  include "dummy.ch"
#endif
