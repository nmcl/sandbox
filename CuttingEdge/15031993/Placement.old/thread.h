/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef THREAD_H_
#define THREAD_H_

/* This class defines a template for threads packages which will be used to
 * provide "active" objects in C++. Such objects are not derived from this
 * class, but instead a thread implementation class is derived from this.
 * That class will then define the pure virtual functions, and "active" objects
 * are then derived from that class.
 * Because not every thread package provides an easy way of identifying and locating
 * threads, the Thread class does provide such a scheme through the use of the
 * Identify and Self operations. A linked list of threads is formed and added to
 * whenever a new thread is created.
 */

class Thread
{
public:
    virtual void Suspend () = 0; // How to suspend a thread
    virtual void Resume () = 0; // How to resume a suspended thread
    virtual void Body () = 0; // The 'main' part of the code

    virtual long Current_Thread () const = 0; // Should return some unique thread identity key

    virtual long Identity () const; // Returns the identify of this thread
    static Thread *Self (); // Returns the current thread

protected:
    Thread ();
    virtual ~Thread ();

    long thread_key;

private:
    Thread *next, *prev;
    static Thread *head;
};

#endif
