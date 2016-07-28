/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */


#include <Config/Configure.h>

#ifndef DEFINITIONS_H_
#  include "Definitions.h"
#endif

char *Definitions::InterfaceName = 0;
#ifdef DATABASELOCATION_Q_
char *Definitions::DatabaseLocation = DATABASELOCATION_Q_;
char *Definitions::DatabaseUID = DATABASEUID_Q_;
#else
char *Definitions::DatabaseLocation = 0;
char *Definitions::DatabaseUID = 0;
#endif
