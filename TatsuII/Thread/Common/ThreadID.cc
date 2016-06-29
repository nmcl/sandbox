/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadID.cc,v 1.1 1997/06/09 19:53:05 nmcl Exp $
 */

/*
 * Implementation of simple thread ids 
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef THREADID_H_
#  include <Thread/ThreadID.h>
#endif

ThreadID::ThreadID () : threadUid()
{
}

ThreadID::ThreadID ( const ThreadID& clone ) : threadUid(clone.threadUid)
{
}

ThreadID::~ThreadID ()
{
}

unsigned int ThreadID::hash () const
{
    return threadUid.hash();
}

ostream& ThreadID::print ( ostream& os ) const
{
    return os << threadUid;
}

