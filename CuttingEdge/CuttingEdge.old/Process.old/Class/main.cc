#define Main_

#include "Processor.h"

main()
{
    LWP_Thread::Initialize();

    Processor *a = new Processor(1);
    Processor *b = new Processor(2);
    Scheduler *sc = new Scheduler();

    sc->Resume();
    a->Resume();
    b->Resume();
    Thread::Self()->Suspend();

    return 0;
}
