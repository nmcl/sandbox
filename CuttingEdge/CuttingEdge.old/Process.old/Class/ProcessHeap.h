#ifndef PROCESS_HEAP_
#define PROCESS_HEAP_

#include "common.h"
#include "Element.h"

class ProcessHeap
{
public:
    ProcessHeap();
    ~ProcessHeap();

    void Insert(Process&, boolean prior = false);
    boolean InsertBefore(Process& ToInsert, Process& Before);
    boolean InsertAfter(Process& ToInsert, Process& After);
    Process* Remove(Process* p = 0);

private:
    Element* Head;

    Element* ScanOnTime(Process&, Element*);
    Element* ScanOnProcess(Process&, Element*);
};

#endif

