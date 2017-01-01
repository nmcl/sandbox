/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Array.h,v 1.11 1998/01/12 12:58:09 nmcl Exp $
 */

#ifndef TEMPLATE_ARRAY_H_
#define TEMPLATE_ARRAY_H_

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class ostream;

typedef unsigned int array_t;

template <class T>
class Array
{
public:
    Array (const Array<T>&);
    Array (array_t);
    ~Array ();

    T& operator[] (array_t i);
    T& operator[] (array_t i) const;
    Array<T>& operator= (const Array<T>&);
    Array<T>& operator= (T);

    array_t  getSize () const;
    Boolean  setSize (array_t size);

    Boolean resize (array_t size);

    ostream& print (ostream&) const;
    
private:
    array_t _size;
    T*      _arrayBase;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#   ifndef TEMPLATE_ARRAY_CC_
#       include <Template/Array.cc>
#   endif
#endif

#endif
