/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecList.h,v 1.1 1993/11/03 14:23:57 nmcl Exp $
 */

#ifndef CRRECLIST_H_
#define SRRECLIST_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include "CrashRecR.h"

/*
 *
 * Crash Recovery Record List Class
 *
 */

class CRRecordList
{
public:
    /* Constructor and destructor */

    CRRecordList();
    ~CRRecordList();
    Boolean insert (CrashRecRecord *);
    Boolean remove (CrashRecRecord *);
    Boolean purge ();
    Boolean resolve ();
    ostream& print (ostream& strm) const;
    int getNoEntries();

private:
    CrashRecRecord *first;
    CrashRecRecord *last;
    int noEntries;
};

#endif // CRRECLIST_H_
