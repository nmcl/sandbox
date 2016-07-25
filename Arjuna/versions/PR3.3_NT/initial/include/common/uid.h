/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Uid.h,v 1.4 1995/03/07 16:21:45 ngdp Exp $
 */

#ifndef UID_H_
#define UID_H_ 

/*
 * WARNING:
 * Do NOT move this define
 */

/*
 * Due to problems in guaranteeing order of initialisation plus
 * compiler deficiencies in some compilers we define the NIL_UID
 * as a macro rather than a real object
 */
 
#define NIL_UID (Uid::_arjunaNilUid_())

/*
 * The remainder of this file processed only by the stub generator.
 */

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

// @NoRemote, @AutoMarshall
class Uid
{
public:
    /* Constructors & destructor */

    Uid ();
    Uid (const Uid& copyFrom);
    Uid (const char *uidString, Boolean errsOk = FALSE);
    ~Uid ();

    static const Uid& _arjunaNilUid_ ();
    
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
    static Uint32 hostInetAddr();

    /* private state variables */

    Uint32 hostAddr;		/* internet address */
    Uint32 process;		/* process number */
    Uint32 sec;			/* time */
    Uint32 other;		/* random... */
    
};

#include <Common/Uid.n>

#endif
