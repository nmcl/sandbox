/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneSpecific.h,v 1.1 1997/09/25 15:32:35 nmcl Exp $
 */

#ifndef NONESPECIFIC_H_
#  define NONESPECIFIC_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADSPECIFIC_H_
#  include <Thread/ThreadSpecific.h>
#endif

/*
 *
 * NONE thread system simple thread specfic data
 *
 */

class NoneSpecific : public ThreadSpecific
{
    friend ThreadSpecific;
    
public:
    virtual ~NoneSpecific ();

    virtual Boolean getSpecific (void *&);
    virtual Boolean setSpecific (void *);
    virtual Boolean destroySpecific ();
    
private:
    NoneSpecific ();

};

#endif
