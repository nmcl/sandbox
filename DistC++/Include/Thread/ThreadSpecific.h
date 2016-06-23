/*
 * Copyright (C) 1995-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSpecific.h,v 1.1 1997/09/25 15:28:24 nmcl Exp $
 */

#ifndef THREADSPECIFIC_H_
#define THREADSPECIFIC_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BASICLIST_H_
#  include <Template/BasicList.h>
#endif

/*
 *
 * Thread system: thread specific data
 *
 */

class ThreadMutex;

class ThreadSpecific
{
public:
    virtual ~ThreadSpecific ();
    
    virtual Boolean getSpecific (void *&) = 0;
    virtual Boolean setSpecific (void *) = 0;
    virtual Boolean destroySpecific () = 0;
    
    virtual Boolean valid () { return valid_; }
    
    static ThreadSpecific* create ();

protected:
    ThreadSpecific ();

    void valid (Boolean v) { valid_ = v; };

    Boolean addToOverflow (void *);
    Boolean getFromOverflow (void *&);
    Boolean removeFromOverflow ();
    
private:
    struct ThreadSpecificData
    {
	ThreadSpecificData (const Uid &u, void *d) : key(u), data(d) {}
	Uid get_uid () { return key; }
    
	Uid key;
	void *data;
    };
    
    Boolean valid_;
    ThreadMutex *lock_;
    BasicList<ThreadSpecific::ThreadSpecificData> overflow;
};

#endif
