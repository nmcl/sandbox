/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Uid.h,v 1.13 1993/04/23 14:24:50 ngdp Exp $
 */

#ifndef UID_H_
#define UID_H_ 

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Unique Identifier implementation
 *
 */

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class Buffer;

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

// @NoRemote, @UserMarshall
class Uid
{
public:
    /* Constructors & destructor */

    Uid ();
    Uid (const Uid& copyFrom);
    Uid (const char *uidString, Boolean errsOk = FALSE);
    ~Uid ();

    /* non-virtual member functions and operators */

    unsigned long hash () const;
    Boolean pack (Buffer& packInto) const;
    ostream& print (ostream& strm) const;
    Boolean unpack (Buffer& unpackFrom);

    int operator== (const Uid& u) const;
    int operator!= (const Uid& u) const;
    int operator<  (const Uid& u) const;
    int operator>  (const Uid& u) const;

private:
    static unsigned long hostInetAddr();

    /* private state variables */

    unsigned long hostAddr;		/* internet address */
    unsigned long process;		/* process number */
    unsigned long sec;			/* time */
    unsigned long other;		/* random... */
    
};

extern const Uid NIL_UID;

#include <Common/Uid.n>

#endif
