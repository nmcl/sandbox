/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSITERATOR_H_
#include "ProcessIterator.h"
#endif


ProcessIterator::ProcessIterator (ProcessList &L) { ptr = L.Head; }

Process *ProcessIterator::operator ()()
{
    if (ptr)
    {
	ProcessCons *p = ptr;
	ptr = ptr->cdr();
	return p->car();
    }

    return 0;
}
