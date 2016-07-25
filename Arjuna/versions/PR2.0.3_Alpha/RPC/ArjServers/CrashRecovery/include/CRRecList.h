/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecList.h,v 1.3 1993/09/08 15:29:17 ndbi Exp $
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
