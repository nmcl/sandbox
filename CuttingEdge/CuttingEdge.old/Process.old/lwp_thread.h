#ifndef LWP_THREAD_H
#define LWP_THREAD_H

#include <lwp/lwp.h>
#include "thread.h"

class LWP_Thread : public Thread
{
public:
    LWP_Thread();
    virtual ~LWP_Thread();

    virtual void Suspend();
    virtual void Resume();
    virtual long Identity();
    virtual void Body() = 0;
    static void Execute(LWP_Thread*);
    thread_t Thread_ID();
    void body
private:
    thread_t mid;
};

#endif
