/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCleanup.h,v 1.2 1996/01/02 13:12:29 nsmw Exp $
 */

#ifndef BASE_THREADCLEANUP_H_
#define BASE_THREADCLEANUP_H_

/*
 * Instances of classes derived from this class perform necessary cleanup
 * actions when the thread is terminated.
 */

class ThreadCleanup
{
public:
    virtual ~ThreadCleanup ();    

    ThreadCleanup* next () const;
    void next (ThreadCleanup*);
    
protected:
    ThreadCleanup ();
    ThreadCleanup (ThreadCleanup*);

private:
    ThreadCleanup* _next;
};


#endif
