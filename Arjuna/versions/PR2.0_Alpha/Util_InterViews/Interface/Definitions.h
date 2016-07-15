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

/* variables which may be set up in configuration script. If they are not
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

#ifdef _INIT_
#undef _INIT_
    char *Definitions::InterfaceName = 0;
#ifdef DATABASELOCATION_Q_
    char *Definitions::DatabaseLocation = DATABASELOCATION_Q_;
    char *Definitions::DatabaseUID = DATABASEUID_Q_;
#else
    char *Definitions::DatabaseLocation = 0;
    char *Definitions::DatabaseUID = 0;
#endif
#endif
#endif
