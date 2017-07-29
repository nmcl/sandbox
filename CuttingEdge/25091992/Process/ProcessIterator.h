#ifndef ProcessIterator_
#define ProcessIterator_

#include "common.h"
#include "Process.h"
#include "ProcessList.h"
#include "ProcessCons.h"

class ProcessIterator {
public:
    ProcessIterator(class ProcessList &L);
    Process *operator()();
private:
    ProcessCons *ptr;
};

#endif
