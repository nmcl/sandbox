/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTSpecific.h,v 1.1 1998/09/16 11:03:29 nmcl Exp $
 */

#ifndef NTSPECIFIC_H_
#define NTSPECIFIC_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADSPECIFIC_H_
#  include <Thread/ThreadSpecific.h>
#endif

#define errorValue 0xFFFFFFFF

class NTSpecific : public ThreadSpecific
{
    friend ThreadSpecific;
    
public:
    virtual ~NTSpecific ();

    virtual Boolean getSpecific (void *&);
    virtual Boolean setSpecific (void *);
    virtual Boolean destroySpecific ();
    
private:
    NTSpecific ();

    DWORD _key;
};

#endif
