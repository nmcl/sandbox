/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSpecific.h,v 1.2 1998/07/21 11:07:57 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADSPECIFIC_H_
#define COMMON_THREAD_THREADSPECIFIC_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

class ostream;

template <class DataType>
class ThreadSpecific
{
public:
    ThreadSpecific (void (*destructor)(DataType*) = 0);
    ~ThreadSpecific ();
    
    Boolean setSpecific    (DataType* threadData);
    Boolean getSpecific    (DataType*& threadData);
    
    Boolean removeSpecific ();
    Boolean deleteSpecific ();

    Boolean valid () const;

    ostream& print (ostream&) const;
    
private:
    ThreadDataImple* _list;
    void (*_destructor)(DataType*);
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#  ifndef COMMON_THREAD_THREADSPECIFIC_CC_
#    include <Common/Thread/ThreadSpecific.cc>
#  endif
#endif

#endif
