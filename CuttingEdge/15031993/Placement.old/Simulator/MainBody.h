/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MAINBODY_H_
#define MAINBODY_H_

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


/* This code initialized and starts the processes which make up the simulation.
 * If -DDEBUG is added to the CFLAGS for the Makefile then the simulation will
 * proceed as normal but information on the number of operations performed will
 * be outputted more regularly. If the -DFAST option is used then the simulation
 * only runs for 1000 successfully performed operations, as oppossed to 10000.
 */

class MainBody : public Process
{
public:
    MainBody (int, char**);
    ~MainBody ();

    void Body ();
    void Await ();

private:
    Machine* Sites[MAX_MACHINES];
    char* policy;
    int NumberOfObjects, NumberOfGoodNodes;
    double mean, sd, availability;
    NormalStream* NS;
    boolean UseAC;
};

#endif
