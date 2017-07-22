/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSITERATOR_H_
#define PROCESSITERATOR_H_

#ifndef COMMON_H_
#include "common.h"
#endif

#ifndef PROCESS_H_
#include "Process.h"
#endif

#ifndef PROCESSLIST_H_
#include "ProcessList.h"
#endif

#ifndef PROCESSCONS_H_
#include "ProcessCons.h"
#endif


class ProcessIterator
{
public:
    ProcessIterator (class ProcessList &L);
    Process *operator ()();

private:
    ProcessCons *ptr;
};

#endif
