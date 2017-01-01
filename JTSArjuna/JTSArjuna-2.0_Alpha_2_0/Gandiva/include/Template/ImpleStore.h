/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ImpleStore.h,v 1.5 1998/01/12 12:58:12 nmcl Exp $
 */

#ifndef TEMPLATE_IMPLESTORE_H_
#define TEMPLATE_IMPLESTORE_H_

class ostream;

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#  include <Common/ObjectName.h>
#endif

template <class T>
class ImplementationElement
{
public:
    ImplementationElement (const ObjectName& name, T* mech);
    virtual ~ImplementationElement ();

    ostream& print (ostream& strm) const;
    
    ObjectName             _objName;
    T*                     _imple;
    ImplementationElement* _next;
};


template <class T>
class ImplementationStore
{
public:
    ImplementationStore ();
    virtual ~ImplementationStore ();

    T* getImplementation (const ObjectName& name);
    
    void addImplementation (const ObjectName& name, T* mech);

    void removeImplementation (T* mech);
    void removeImplementation (const ObjectName& name);
    
    ostream& print (ostream& strm) const;

    Boolean empty () const;
    
private:
    ImplementationElement<T>* headOfList;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#   ifndef TEMPLATE_IMPLESTORE_CC_
#       include <Template/ImpleStore.cc>
#   endif
#endif

#endif
