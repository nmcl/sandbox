/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockListI.cc,v 1.2 1993/11/16 09:55:36 ngdp Exp $
 */

/*
 *
 * Lock iterator class
 *
 */ 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockListI.h"
#endif

#ifndef LOCK_H_
#  include <Arjuna/Lock.h>
#endif

static const char RCSid[] = "$Id: LockListI.cc,v 1.2 1993/11/16 09:55:36 ngdp Exp $";

/*
 * Public constructors and destructors 
 */

LockListI::LockListI ( const LockList& L )
                     : currentList(L),
		       next(L.head)
{ 
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockListI::LockListI(" << (void *)&L << ")" << endl;
#endif
}

LockListI::~LockListI ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockListI::~LockListI()" << endl;
#endif

    next = 0;
}

Lock *LockListI::operator()()
{
    Lock *current = next;
    if (current == 0)
    {
	return 0;
    }
    else
	next = current->getLink();

    return current;
}

#ifdef NO_INLINES
#  define LOCKLISTI_CC_
#  include "LockListI.n"
#  undef LOCKLISTI_CC_
#endif
