#ifndef ProcessList_
#define ProcessList_

#include "common.h"
#include "Process.h"
#include "ProcessIterator.h"
#include "ProcessCons.h"

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
