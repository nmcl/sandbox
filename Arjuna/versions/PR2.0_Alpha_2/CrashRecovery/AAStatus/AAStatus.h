/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AAStatus.h,v 1.1 1993/11/03 14:16:48 nmcl Exp $
 */

#ifndef AASTATUS_H_
#define AASTATUS_H_

/*
 *
 * Atomic Action Status Class
 *
 */

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

class AAStatus
{
public:
    AAStatus ();
    ~AAStatus ();

    Boolean isCommitted (Uid);

};

#endif // AASTATUS_H_
