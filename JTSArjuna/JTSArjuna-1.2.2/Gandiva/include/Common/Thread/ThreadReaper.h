/*
 * Copyright (C) 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadReaper.h,v 1.1 1998/01/12 12:56:12 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADREAPER_H_
#define COMMON_THREAD_THREADREAPER_H_

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

class ostream;
class Mutex;

class ThreadReaper
{
public:
    ThreadReaper (size_t numberToReap = 1);
    virtual ~ThreadReaper ();

    Boolean add (Thread*);
    
    void purge ();

    long size () const;  // -1 signals an error

    virtual ostream& print (ostream&) const;
    
private:
    Uid                      _id;
    size_t                   _numberToReap;
    Mutex*                   _mutex;
    HashedList<Thread, Uid>* _threadList;
};

extern ostream& operator<< (ostream& strm, const ThreadReaper& t);

#endif
