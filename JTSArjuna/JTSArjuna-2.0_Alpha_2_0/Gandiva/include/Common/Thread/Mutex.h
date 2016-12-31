/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Mutex.h,v 1.1 1998/01/12 12:56:05 nmcl Exp $
 */

#ifndef COMMON_THREAD_MUTEX_H_
#define COMMON_THREAD_MUTEX_H_

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

class ostream;
class MutexData;

class Mutex : public Resource
{
public:
    enum MutexStatus { MX_DONE, MX_ERROR, MX_WOULD_BLOCK };

    virtual MutexStatus lock    ();
    virtual MutexStatus unlock  ();
    virtual MutexStatus tryLock ();

    const Uid& uid () const;
    
    static Mutex* create (MutexSetup* = 0);
    
    virtual ostream& print (ostream&) const;
    
protected:
    Mutex (MutexSetup* = 0);
    virtual ~Mutex ();

    /*
     * Here so mutex/semaphores can initialize the threads package
     * if they are used before any threads are created.
     */
    
    static void initialize ();

private:
    Uid _mutexId;
    MutexData* _mutexData;
    
    static ClassName* _className;

public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*  castup (const ClassName&) const;
    static  Mutex* castup (Resource*);
};

#include <Common/Thread/Mutex.n>

#endif
