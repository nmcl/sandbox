#define Main_

#include <iostream.h>
#include <lwp/lwp.h>
#include "Processor.h"
#include "common.h"

main()
{
    (void) pod_setmaxpri(MAXPRIO);
    (void) lwp_setstkcache(1000, 5);

    Processor *a = new Processor(1);
    Processor *b = new Processor(2);

    a->Resume();
    b->Resume();
    (void) lwp_create((thread_t*)0, Scheduler, MINPRIO, 0, lwp_newstk(), 0);

    lwp_suspend(SELF);
}
