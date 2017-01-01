/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicList.h,v 1.7 1998/01/19 12:37:30 nmcl Exp $
 */

#ifndef TEMPLATE_BASICLIST_H_
#define TEMPLATE_BASICLIST_H_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#    include <Common/Boolean.h>
#endif

template <class T, class U> class BasicListIterator;

template <class T>
class ListCons
{
public:
    ListCons (T* p, ListCons<T> *n);
    ~ListCons ();
    
    T*           car ();
    ListCons<T> *cdr ();
    void SetfCdr (ListCons<T>* n);

private:
    T*           content;
    ListCons<T>* next;
};

/*
 * Make thread safe? Problem is: if each operation acquires and
 * releases the lock, then an iterator cannot guarantee the list
 * will not change between each invocation of ().
 */

template <class T, class U>
class BasicList
{
    friend BasicListIterator<T, U>;
    
public:
    BasicList ();
    ~BasicList ();

    void    insert (T*);
    T*      remove (const U&);
    Boolean remove (T*);
    T*      find   (const U&) const;

    ListCons<T>* operator[] (int);

    ostream& print (ostream&) const;

private:
    ListCons<T>* Head;
};

template <class T, class U>
inline ostream& operator<< (ostream& strm, const BasicList<T, U>& pl)
{
    return pl.print(strm);
}

#ifndef HAVE_TEMPLATE_REPOSITORY
#    ifndef TEMPLATE_BASICLIST_CC_
#        include <Template/BasicList.cc>
#    endif
#endif

#endif
