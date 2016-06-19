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


enum protocolTypes { active, passive, voting, single };


class MainBody : public Process
{
public:
    MainBody (int, char**);
    ~MainBody ();

    void Body ();
    void Await ();

private:
    double mean, sd;
    protocolTypes toUse;
    NormalStream* NS;
};

#endif
