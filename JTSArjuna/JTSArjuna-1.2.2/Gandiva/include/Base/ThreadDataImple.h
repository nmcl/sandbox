/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadDataImple.h,v 1.3 1998/01/12 12:53:47 nmcl Exp $
 */

#ifndef BASE_THREADDATAIMPLE_H_
#define BASE_THREADDATAIMPLE_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef TEMPLATE_BASICLIST_H_
#  include <Template/BasicList.h>
#endif

class Mutex;

#if defined(NO_NESTED_CLASSES) || defined(WIN32)
struct ThreadSpecificData
{
    ThreadSpecificData (const Uid &u, void *d) : key(u), data(d) {};
    Uid uid () { return key; };
    
    Uid key;
    void *data;
};
#endif

class ThreadDataImple
{
public:
    virtual ~ThreadDataImple ();
    
    virtual Boolean setSpecific    (void* threadData) = 0;
    virtual Boolean getSpecific    (void*& threadData) = 0;
    virtual Boolean removeSpecific () = 0;

    virtual Boolean valid () const { return valid_; };
    
    static ThreadDataImple* create ();
    
protected:
    ThreadDataImple ();

    void valid (Boolean v) { valid_ = v; };

    Boolean addToOverflow (void *);
    Boolean getFromOverflow (void *&);
    Boolean removeFromOverflow ();

private:
#if !defined(NO_NESTED_CLASSES) && !defined(WIN32)
    struct ThreadSpecificData
    {
	ThreadSpecificData (const Uid &u, void *d) : key(u), data(d) {};
	Uid uid () { return key; };
    
	Uid key;
	void *data;
    };
#endif
    
    Boolean valid_;
    Mutex* lock_;

#if !defined(NO_NESTED_CLASSES) && !defined(WIN32)
    BasicList<ThreadDataImple::ThreadSpecificData, Uid> overflow;
#else
    BasicList<ThreadSpecificData, Uid> overflow;
#endif
};

#endif
