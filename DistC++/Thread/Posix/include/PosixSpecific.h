/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixSpecific.h,v 1.1 1997/09/25 15:32:52 nmcl Exp $
 */

#ifndef POSIXSPECIFIC_H_
#  define POSIXSPECIFIC_H_

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADSPECIFIC_H_
#  include <Thread/ThreadSpecific.h>
#endif

/*
 *
 * POSIX thread specific data.
 *
 */

class PosixSpecific : public ThreadSpecific
{
    friend ThreadSpecific;
    
public:
    virtual ~PosixSpecific ();

    virtual Boolean getSpecific (void *&);
    virtual Boolean setSpecific (void *);
    virtual Boolean destroySpecific ();
    
private:
    PosixSpecific ();

    pthread_key_t key_;
};

#endif
