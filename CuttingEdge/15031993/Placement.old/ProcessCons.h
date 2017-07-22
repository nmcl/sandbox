/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSCONS_H_
#define PROCESSCONS_H_

#ifndef COMMON_H_
#include "common.h"
#endif

#ifndef PROCESS_H_
#include "Process.h"
#endif


class ProcessCons
{
public:
    ProcessCons (Process &p, ProcessCons *n);
    Process     *car ();
    ProcessCons *cdr ();
    void SetfCdr (ProcessCons *n);

private:
    Process *Proc;
    ProcessCons *Next;
};


#include "ProcessCons.n"

#endif
