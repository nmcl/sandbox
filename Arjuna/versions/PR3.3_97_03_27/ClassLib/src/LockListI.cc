/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockListI.cc,v 1.5 1995/06/26 13:15:57 ngdp Exp $
 */

/*
 *
 * Lock iterator class
 *
 */ 

#ifdef sun
/* Get over standard cpp include nesting limit on 4.1.* */
#    ifndef STATEMAN_H_
#      include <Arjuna/StateMan.h>
#    endif
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

static const char RCSid[] = "$Id: LockListI.cc,v 1.5 1995/06/26 13:15:57 ngdp Exp $";

/*
 * Public constructors and destructors 
 */

LockListI::LockListI ( const LockList& L )
                     : currentList(L),
		       next(L.head)
{ 
}

LockListI::~LockListI ()
{
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
