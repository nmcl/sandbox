#ifndef ProcessCons_
#define ProcessCons_

#include "common.h"
#include "Process.h"

class ProcessCons {
public:
    ProcessCons (Process &p, ProcessCons *n);
    Process     *car ();
    ProcessCons *cdr ();
    void SetfCdr(ProcessCons *n);
private:
    Process *Proc;
    ProcessCons *Next;
};

#endif
