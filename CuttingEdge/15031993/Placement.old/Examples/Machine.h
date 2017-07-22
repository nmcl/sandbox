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

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef PROCESS_H_
#include <Process.h>
#endif


/* This is the machine which services job requests. It is prone to simulated
 * failures caused by the Breaks process.
 */

class Machine : public Process
{
public:
    Machine (double);
    ~Machine ();

    void Body ();

    void Broken ();
    void Fixed ();
    boolean IsOperational ();
    boolean Processing ();
    double ServiceTime ();

private:
    ExponentialStream* STime;
    boolean operational;
    boolean working;
};

#endif
