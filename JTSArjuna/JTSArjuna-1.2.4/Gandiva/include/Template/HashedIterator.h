/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedIterator.h,v 1.5 1998/01/12 12:58:11 nmcl Exp $
 */

#ifndef TEMPLATE_HASHEDITERATOR_H_
#define TEMPLATE_HASHEDITERATOR_H_

template <class T, class U> class HashedIterator;

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR
#  include <Template/BasicListIterator.h>
#endif

template <class T, class U>
class HashedIterator
{
public:
    HashedIterator (const HashedList<T,U>& H);
    ~HashedIterator ();

    T* operator ()();

private:
    BasicListIterator<T,U>* ptr;
    const HashedList<T,U>& ref;
    unsigned long _index;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#   ifndef TEMPLATE_HASHEDITERATOR_CC_
#       include <Template/HashedIterator.cc>
#   endif
#endif

#endif
