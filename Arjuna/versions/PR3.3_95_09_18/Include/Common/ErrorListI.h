/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ErrorListI.h,v 1.2 1994/10/18 08:14:20 ngdp Exp $
 */

#ifndef LOCKLISTI_H_
#define LOCKLISTI_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Error iterator class
 *
 */

class Error;
class ErrorList;

// @NoRemote, @NoMarshall
class ErrorListI
{
public:
    /* Constructors and destructor */

    ErrorListI (const ErrorList& L);
    ~ErrorListI ();

    /* non-virtual functions and operators */

    void reset ();
    const Error * const operator() ();

private:
    /* private state variables */
    
    const ErrorList& currentList;	/* list we are curently walking */
    Error *next;			/* return this one next time */
};

#include <Common/ErrorListI.n>

#endif
