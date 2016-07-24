/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecList.cc,v 1.1 1993/11/03 14:16:59 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef CRRECLIST_H_
#  include "CRRecList.h"
#endif

CRRecordList::CRRecordList ()
                           : first(0),
                             last(0),
                             noEntries(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::CRRecordList ()\n" << flush;
#endif
}

CRRecordList::~CRRecordList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::~CRRecordList ()\n" << flush;
#endif
}

int CRRecordList::getNoEntries()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::getNoEntries\n" << flush;
#endif

    return noEntries;
}

Boolean CRRecordList::insert (CrashRecRecord *newRecord)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::insert (CrashRecRecord *)\n" << flush;
#endif

    if (noEntries == 0)
    {
        first = last = newRecord;
        noEntries = 1;
    }
    else
    {
        last->next = newRecord;
        last = newRecord;
        noEntries++;
    }
    return TRUE;
}

Boolean CRRecordList::remove (CrashRecRecord *p)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::remove (CrashRecRecord *)" << endl;
#endif

    cout << "Removing: ";
    p->id(cout);

    if (p == first)
        first = p->next;

    if (p == last)
        last = p->previous;

    delete p;
    noEntries--;

    return TRUE;
}    


ostream& CRRecordList::print (ostream& strm ) const
{
    if (noEntries != 0)
    {
        strm << "----------------------------------------" << endl;
        strm << "Number of entries in list = " << noEntries << endl;

        CrashRecRecord *temp;
        temp = first;
        while (temp != 0)
        {
            strm << "----------------------------------------" << endl;
            temp->print(strm);
            temp = temp->next;
        }
        strm << "----------------------------------------" << endl;

    }
    else
        strm << "** LIST IS EMPTY **" << endl;

    return (strm);
}

Boolean CRRecordList::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::resolve()\n" << flush;
#endif

    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = first;
        while (temp != 0)
        {
            temp->resolve();
            temp = temp->next;
        }
    }
    return(TRUE);
}


Boolean CRRecordList::purge()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::purge()\n" << flush;
#endif

    cout << "Purging record list..." << endl;

    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = first;
        while (temp != 0)
        {
            if ( temp->getRecoveryStatus() == COMPLETE )
                remove(temp);

            temp = temp->next;
        }
    }

    cout << "Purge complete" << endl;

    return(TRUE);
}
