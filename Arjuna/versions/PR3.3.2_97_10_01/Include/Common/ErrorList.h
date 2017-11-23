/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ErrorList.h,v 1.4 1994/10/18 08:14:18 ngdp Exp $
 */

#ifndef ERRORLIST_H_

#ifndef STUB
#  include <Common/ErrorList_stub.h>
#else

#define ERRORLIST_H_

/*
 *
 * Lock list maintainance class interface
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

// @NoRemote
class ErrorList
{
    // @NoRemote, @NoMarshall
    friend class ErrorListI;	/* allow access to private members */

public:
    /* Constructors and destructor */
    ErrorList ();
    ~ErrorList ();

    /* non-virtual member functions and operators */

    void clearList ();
    const Error *const findError (FacilityCode) const;
    int entryCount () const;
    Boolean insert (Error *);
    Error *pop ();
    void push (Error *newError);
    void forgetNext (Error *current);

private:
    /* private state variables */

    int count;				/* current number of members */
    Error *head;			/* head of list */
    
};

#include <Common/ErrorList.n>

#endif
#endif
