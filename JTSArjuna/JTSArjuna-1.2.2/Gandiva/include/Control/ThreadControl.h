/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadControl.h,v 1.1 1996/06/04 12:03:37 nmcl Exp $
 */

#ifndef CONTROL_THREADCONTROL_H_
#define CONTROL_THREADCONTROL_H_

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

class Thread;

/*
 * Control methods which are supported only be certain thread packages.
 */

class ThreadControl : public Resource
{
public:
    ThreadControl (Thread*);
    virtual ~ThreadControl ();

    Boolean setPriority  (int priority);
    
private:
    Thread* _toControl;
    
public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*          castup (const ClassName&) const;
    static  ThreadControl* castup (Resource*);
};

#endif
