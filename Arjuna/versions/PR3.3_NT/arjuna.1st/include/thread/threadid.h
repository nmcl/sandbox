/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadID.h,v 1.1 1995/06/26 10:38:57 ngdp Exp $
 */

#ifndef THREADID_H_
#define THREADID_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

/*
 *
 * Thread system thread id's. Map to Uid's
 *
 */

class ostream;

class ThreadID
{
public:
    ThreadID ();
    ThreadID (const ThreadID&);
    virtual ~ThreadID ();

    virtual unsigned int hash () const;
    virtual ostream& print (ostream&) const;
    
    virtual Boolean operator== (const ThreadID&) const ;
    virtual Boolean operator!= (const ThreadID&) const;
    
private:
    Uid threadUid;
};

inline Boolean ThreadID::operator== ( const ThreadID& other ) const
{
    return (threadUid == other.threadUid);
}

inline Boolean ThreadID::operator!= ( const ThreadID& other ) const
{
    return !(*this == other);
}

inline ostream& operator<< ( ostream& os, const ThreadID& id )
{
    return id.print(os);
}

#endif
