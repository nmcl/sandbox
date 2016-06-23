/*
 * Copyright (C) 1995, 1996, 1997
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicList.h,v 1.1 1997/09/25 15:27:55 nmcl Exp $
 */

#ifndef BASICLIST_H_
#define BASICLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SIMPLELIST_H_
#  include <Template/SimpleList.h>
#endif

template <class T> class BasicList;
template <class T> class BasicListI;

template <class T>
class BasicList : public SimpleList<T>
{
    friend BasicListI<T>;
    
public:
    BasicList ();
    virtual ~BasicList ();
    
    T* remove (const Uid&);
    T* lookFor (const Uid&);

    Boolean remove (T*);
    Boolean lookFor (T*);
};

template <class T>
class BasicListI : public SimpleListI<T>
{
public:
    BasicListI (const BasicList<T>&);
};

#ifdef __GNUG__

#ifndef BASICLIST_CC_
#  include <Template/BasicList.cc>
#endif

#endif

#endif
