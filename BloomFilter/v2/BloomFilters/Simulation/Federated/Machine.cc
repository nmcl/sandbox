/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif

Machine::Machine(long id): machineID(id), operational(TRUE), working(FALSE)
{}

Machine::~Machine ()
{}

void Machine::stopWorking()
{
    working = FALSE;
}

void Machine::startWorking()
{
    working = TRUE;
}

Boolean Machine::isOperational()
{
   return operational;
}

void Machine::breakIt()
{
    operational = FALSE;
}

void Machine::fixIt()
{
    operational = TRUE;
}
