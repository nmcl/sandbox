#ifndef THREAD_H
#define THREAD_H

class Thread
{
public:
    virtual void Suspend() = 0;
    virtual void Resume() = 0;
    virtual void Body() = 0;

    virtual long Current_Thread()=0;

    virtual long Identity();
    static Thread *Self();

private:
    Thread *next, *prev;
    static Thread *head;

protected:
    Thread();
    ~Thread();

    long thread_key;
};

#ifdef INIT_
#undef INIT_
    class Thread *Thread::head = 0;
#endif
#endif
