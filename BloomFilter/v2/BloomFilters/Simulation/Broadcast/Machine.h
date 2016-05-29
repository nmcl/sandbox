/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif

// Machine is an Abstract Base Class. Any particular machine must
// specify a Body() function.

class Machine : public Process
{
    friend class Breaks;

protected:
    Machine (long id);
    virtual ~Machine ();

    virtual void Body () = 0;  
    
    void breakIt();
    void fixIt();
    Boolean isOperational();
    void startWorking();
    void stopWorking();

    long machineID;
private:
    Boolean operational;
    Boolean working;
};

#endif
