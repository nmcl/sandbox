/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedList.h,v 1.7 1998/01/12 12:58:12 nmcl Exp $
 */

#ifndef TEMPLATE_HASHEDLIST_H_
#define TEMPLATE_HASHEDLIST_H_

template <class T, class U> class HashedList;

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef TEMPLATE_BASICLIST_H_
#  include <Template/BasicList.h>
#endif

const unsigned long defaultSize = 77;

template <class T, class U> class HashedIterator;

template <class T, class U>
class HashedList
{
    friend HashedIterator<T, U>;

public:
    HashedList (unsigned long size = defaultSize);
    ~HashedList ();

    void                   insert (T*, const U& key);
    Boolean                remove (T*, const U& key);
    const BasicList<T, U>* find   (const U& key) const;

    ostream& print (ostream&) const;

    unsigned long size () const;
    
private:
    int idToKey (const U&) const;

    BasicList<T, U>**     hashedList;
    const unsigned long   listSize;
    unsigned long         numberOfEntries;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#   ifndef TEMPLATE_HASHEDLIST_CC_
#       include <Template/HashedList.cc>
#   endif
#endif

#endif
