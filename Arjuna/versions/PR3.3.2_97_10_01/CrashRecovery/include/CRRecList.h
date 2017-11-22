/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecList.h,v 1.4 1994/08/04 08:59:52 ndbi Exp $
 */

#ifndef CRRECLIST_H_
#define CRRECLIST_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CRASHRECR_H_
#  include <CrashRecR.h>
#endif

#ifndef CRRECTYPE_H_
#  include <CRRecType.h>
#endif

/*
 *
 * Crash Recovery Record List Class
 *
 */

class CRRecordList
{
public:
    /* Constructor and destructor */

    CRRecordList(ostream&);
    ~CRRecordList();

    Boolean scan (CrashRecRecord *);
    Boolean insert (CrashRecRecord*);
    Boolean remove (CrashRecRecord*);
    Boolean purge ();
    Boolean resolve ();
    void print (ostream& strm);
    int getNoEntries();
    int getNoEntries(CRRecordType);
    int getWorkLevel();
    int getWorkLevel(CRRecordType);
    void printWorkIds (ostream&);

private:
    ostream& outStrm;
    CrashRecRecord *listHead;
    CrashRecRecord *listTail;
    int noEntries;
};

#endif // CRRECLIST_H_
