#ifndef LWP_THREAD_H
#define LWP_THREAD_H

#include <lwp/lwp.h>
#include "thread.h"


class LWP_Thread : public Thread
{
protected:
    static const int MaxPriority;
    LWP_Thread(int priority=MaxPriority);
    LWP_Thread(thread_t);
public:
    virtual void Suspend();
    virtual void Resume();
    virtual void Sleep(struct timeval);
    virtual void Body() = 0;
    virtual long Current_Thread();

    thread_t Thread_ID();

    // Initialize must be called exactly once at the start of the program
    static void Initialize();

private:
    virtual ~LWP_Thread();
    static void Execute(LWP_Thread*);
    thread_t mid;
};

#endif
