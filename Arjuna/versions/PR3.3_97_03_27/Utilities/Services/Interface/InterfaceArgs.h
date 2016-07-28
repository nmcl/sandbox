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

#ifndef INTERFACEARGS_H_
#define INTERFACEARGS_H_

#ifndef DEFINITIONS_H_
#include "Definitions.h"
#endif

class InterfaceArgs
{
public:
    InterfaceArgs ();
    ~InterfaceArgs ();

    static Boolean CheckArgs (int, char**, Boolean = TRUE);
};

#endif
