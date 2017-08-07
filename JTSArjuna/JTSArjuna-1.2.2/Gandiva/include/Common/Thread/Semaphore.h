/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Semaphore.h,v 1.1 1998/01/12 12:56:08 nmcl Exp $
 */

#ifndef COMMON_THREAD_SEMAPHORE_H_
#define COMMON_THREAD_SEMAPHORE_H_

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORESETUP_H_
#  include <Common/Thread/SemaphoreSetup.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

class ostream;
class SemaphoreData;

class Semaphore : public Resource
{
public:
    enum SemaphoreStatus { SEM_DONE, SEM_ERROR, SEM_WOULD_BLOCK };

    virtual SemaphoreStatus lock    ();
    virtual SemaphoreStatus unlock  ();
    virtual SemaphoreStatus tryLock ();

    const Uid& uid () const;
    
    static Semaphore* create (unsigned int numb, SemaphoreSetup* = 0);

    virtual ostream& print (ostream&) const;
    
protected:
    Semaphore (unsigned int, SemaphoreSetup* = 0);
    virtual ~Semaphore ();

    /*
     * Here so mutex/semaphores can initialize the threads package
     * if they are used before any threads are created.
     */
    
    static void initialize ();

private:
    Uid _semId;
    SemaphoreData* _semaphoreData;
    
    static ClassName* _className;

public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*      castup (const ClassName&) const;
    static  Semaphore* castup (Resource*);    
};

#include <Common/Thread/Semaphore.n>

#endif
