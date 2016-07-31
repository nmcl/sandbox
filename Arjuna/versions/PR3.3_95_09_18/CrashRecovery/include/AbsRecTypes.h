/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbsRecTypes.h,v 1.2 1995/02/22 14:55:18 ngdp Exp $
 */

#ifndef ABSRECTYPES_H_
#define ABSRECTYPES_H_

/*
 * These files are the abstract records which are required for crash
 * recovery. They are the only ones for which save_state and restore_state
 * have any meaning.
 * We have to pre-include these files here even though static types exist
 * within the various libraries. This is because current C++ compilers do
 * not implement the standard regarding static variables and initialization
 * when the program starts. This is currently the only way to guarantee that
 * these types are created before they are used.
 */

#ifndef ABSTRACTR_H_
#  include "AbstractR.h"
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

#ifndef RECOVERYR_H_
#  include "RecoveryR.h"
#endif

#ifndef RAJCALLR_H_
#  include "RajCallR.h"
#endif

#ifndef REPLICATIONR_H_
#  include "ReplicationR.h"
#endif

#ifndef NAMINGR_H_
#  include "NamingR.h"
#endif

#endif
