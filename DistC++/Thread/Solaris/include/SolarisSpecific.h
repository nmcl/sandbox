/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisSpecific.h,v 1.1 1997/09/25 15:32:59 nmcl Exp $
 */

#ifndef SOLARISSPECIFIC_H_
#  define SOLARISSPECIFIC_H_

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADSPECIFIC_H_
#  include <Thread/ThreadSpecific.h>
#endif

/*
 *
 * SOLARIS thread system simple mutex
 *
 */

class SolarisSpecific : public ThreadSpecific
{
    friend ThreadSpecific;
    
public:
    virtual ~SolarisSpecific ();

    virtual Boolean getSpecific (void *&);
    virtual Boolean setSpecific (void *);
    virtual Boolean destroySpecific ();
    
private:
    SolarisSpecific ();

    thread_key_t key_;
};

#endif
