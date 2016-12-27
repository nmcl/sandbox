/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SemData.h,v 1.3 1998/01/12 12:58:15 nmcl Exp $
 */

#ifndef TEMPLATE_SEMDATA_H_
#define TEMPLATE_SEMDATA_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_ARRAY_H_
#  include <Template/Array.h>
#endif

template <class T>
class SemData
{
public:
    SemData (array_t);
    virtual ~SemData ();

    T& operator[] (array_t i);
    T& operator[] (array_t i) const;
    
    virtual Boolean isOwner (const T& me, array_t& index) const;
    virtual Boolean addOwner (const T& me, array_t& index);
    
    virtual void reset (array_t);

    Boolean full () const;
    
    Boolean increment (array_t index);
    Boolean decrement (array_t index);

    array_t size () const;
    
    int count (array_t) const;
    int totalCount () const;
    
    Boolean dead () const;
    void dead (Boolean);
    
protected:
    Boolean    _dead;
    array_t    _numberAllowed;
    Array<int> _useCount;
    Array<T>   _currentOwners;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#  include <Template/SemData.cc>
#endif

#endif
