/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicListIterator.h,v 1.6 1998/01/12 12:58:11 nmcl Exp $
 */

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#define TEMPLATE_BASICLISTITERATOR_H_

template <class T, class U> class BasicListIterator;

#ifndef TEMPLATE_BASICLIST_H_
#  include <Template/BasicList.h>
#endif

template <class T, class U>
class BasicListIterator
{
public:
    BasicListIterator (const BasicList<T, U>& L);

    T* operator ()();

private:
    ListCons<T>* ptr;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#    ifndef TEMPLATE_BASICLISTITERATOR_CC_
#        include <Template/BasicListIterator.cc>
#    endif
#endif

#endif
