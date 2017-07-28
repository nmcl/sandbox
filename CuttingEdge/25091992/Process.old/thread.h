#ifndef THREAD_H
#define THREAD_H

class Thread
{
    static Thread *head;
public:
    Thread();
    ~Thread();

    virtual void Suspend() = 0;
    virtual void Resume() = 0;
    virtual long Identity() = 0;
    virtual void Body() = 0;
    static Thread *Self();

    long thread_key;
    Thread *next, *prev;
};

#ifdef INIT_
#undef INIT_
    class Thread *Thread::head = 0;
#endif
#endif
