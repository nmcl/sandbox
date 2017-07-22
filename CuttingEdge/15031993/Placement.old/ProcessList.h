/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSLIST_H_
#define PROCESSLIST_H_

#ifndef COMMON_H_
#include "common.h"
#endif

#ifndef PROCESS_H_
#include "Process.h"
#endif

#ifndef PROCESSITERATOR_H_
#include "ProcessIterator.h"
#endif

#ifndef PROCESSCONS_H_
#include "ProcessCons.h"
#endif


class ProcessList
{
public:
    ProcessList ();
    void Insert (Process &, boolean prior=false);
    boolean InsertBefore (Process &ToInsert, Process &Before);
    boolean InsertAfter (Process &ToInsert, Process &After);
    Process *Remove (const Process *p=0);

private:
    friend class ProcessIterator;
    ProcessCons *Head;
};

#endif
