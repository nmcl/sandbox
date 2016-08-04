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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/*
 * Variables which may be set up in configuration script. If they are not
 * set up there, or not (re)defined in the program then any server
 * created will default to the home node, and a new persistent object
 * will be created.
 */

class Definitions
{
public:
    Definitions () {};
    ~Definitions () {};

    static char *InterfaceName;
    static char *DatabaseLocation;
    static char *DatabaseUID;
};


#endif
