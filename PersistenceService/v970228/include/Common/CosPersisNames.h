/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef COMMON_COSPERSISNAMES_H_
#define COMMON_COSPERSISNAMES_H_

/*
 * Simple renaming macros for portability
 *
 * $Id$
 */

#ifndef COMMON_NAMESPACE_H_
#  include <Common/nameSpace.h>
#endif

#ifndef COMMONCORBANAMES_H_
#  include <Common/CorbaNames.h>
#endif

#define COS_PDS(name) SCOPED_NAME(CosPersistencePDS,name)
#define COS_PID(name) SCOPED_NAME(CosPersistencePID,name)
#define COS_POM(name) SCOPED_NAME(CosPersistencePOM,name)

/*
 * CosPersistence modules types
 */

typedef COS_PID(PID) PID;
typedef COS_PDS(PDS) PDS;
typedef COS_POM(POM) POM;

typedef COS_PID(PID_ptr) PID_ptr;
typedef COS_PDS(PDS_ptr) PDS_ptr;
typedef COS_POM(POM_ptr) POM_ptr;

typedef COS_PID(PID_var) PID_var;
typedef COS_PDS(PDS_var) PDS_var;
typedef COS_POM(POM_var) POM_var;

/*
 * CosPersistence modules exceptions
 */

#endif
