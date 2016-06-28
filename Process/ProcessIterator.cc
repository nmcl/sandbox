#include "ProcessIterator.h"

ProcessIterator::ProcessIterator(ProcessList &L)
{
    ptr = L.Head;
}

Process *ProcessIterator::operator()()
{
    if (ptr)
    {
	ProcessCons *p = ptr;
	ptr = ptr->cdr();
	return p->car();
    }

    return 0;
}
