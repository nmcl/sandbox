/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRRecList.cc,v 1.3 1994/08/30 08:17:30 ngdp Exp $
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
#  include <CRRecList.h>
#endif

CRRecordList::CRRecordList (ostream& strm)
                           : outStrm(strm),
                             listHead(0),
                             listTail(0),
                             noEntries(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::CRRecordList()\n" << flush;
#endif
}

CRRecordList::~CRRecordList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::~CRRecordList()\n" << flush;
#endif

    while(listHead != 0)
        remove(listHead);

}

Boolean CRRecordList::scan(CrashRecRecord *newRecord)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::scan()" << endl;
#endif
    Boolean found = FALSE;
    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if ( *temp == newRecord)
                found = TRUE;

            temp = temp->next;
        }
    }
    return(found);
}

Boolean CRRecordList::insert(CrashRecRecord *newRecord)
{
    scan(newRecord);

    if (noEntries == 0)
    {
        // Insert as only element
        listHead = listTail = newRecord;
        noEntries = 1;
    }
    else
    {
        Boolean inserted = FALSE;
        CrashRecRecord *temp = listHead;

        while (!inserted)
        {
            if (newRecord->type() < temp->type())
            {
                newRecord->next = temp;
                newRecord->previous = temp->previous;

                if (temp->previous)
                {
                    temp->previous->next = newRecord;
                }
                else
                {
                    // Insert as listHead element
                    listHead = newRecord;
                }
                temp->previous = newRecord;
                inserted = TRUE;
            }
            else
            {
                if (temp->next)
                    temp = temp->next;
                else
                {
                    //Insert as listTail element
                    temp->next = newRecord;
                    newRecord->previous = temp;
                    listTail = newRecord;
                    inserted = TRUE;
                }
            }
        }
        noEntries++;
    }
    return TRUE;
}




int CRRecordList::getNoEntries()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::getNoEntries()" << endl;
#endif

    return (noEntries);
}

int CRRecordList::getNoEntries(CRRecordType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::getNoEntries(" << t << ")" << endl;
#endif

    int count = 0;

    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if (temp->type() == t)
                count++;

            temp = temp->next;
        }
    }

    return(count);
}

int CRRecordList::getWorkLevel()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::getWorkLevel()" << endl;
#endif

    int count = 0;
    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if (temp->getRecoveryStatus() != COMPLETE)
                count++;
            temp = temp->next;
        }
    }
    return(count);
}

int CRRecordList::getWorkLevel(CRRecordType t)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::getWorkLevel(" << t << ")" << endl;
#endif

    int count = 0;
    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if ((temp->type() == t) && (temp->getRecoveryStatus() != COMPLETE))
                count++;
            temp = temp->next;
        }
    }
    return(count);
}

void CRRecordList::printWorkIds(ostream& strm)
{
    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if (temp->getRecoveryStatus() != COMPLETE)
                temp->id(strm);
            temp = temp->next;
        }
    }
}

Boolean CRRecordList::remove (CrashRecRecord *p)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::remove (CrashRecRecord *)" << endl;
#endif

    if (p == listHead)
        listHead = p->next;

    if (p == listTail)
        listTail = p->previous;

    delete p;
    noEntries--;

    return TRUE;
}    


void CRRecordList::print (ostream& strm)
{
    if (noEntries != 0)
    {
        strm << "----------------------------------------" << endl;
        strm << "Number of entries in list = " << noEntries << endl;

        CrashRecRecord *temp;
        temp = listHead;
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
}

Boolean CRRecordList::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::resolve()" << endl;
#endif

    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;

        // Skip over the completed ones to find the first unresolved one
        while ( (temp != 0) && (temp->getRecoveryStatus() == COMPLETE) )
            temp = temp->next;

        if (temp != 0)
        {
            CrashRecRecord *firstWork = temp;
            Boolean resolvedAll = TRUE; // Resolved all of this type of record?

            if ( !(temp->resolve()) )
                resolvedAll = FALSE;
            temp = temp->next;

            while( (temp != 0) && (temp->type() == firstWork->type()) )
            {
                if (temp->getRecoveryStatus() != COMPLETE)
                {
                    if ( !(temp->resolve()) )
                        resolvedAll = FALSE;
                }
                temp = temp->next;
            }
            if ( (temp != 0) && resolvedAll )
                resolve();
        }
    }
    return(TRUE);
}

Boolean CRRecordList::purge()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRRecordList::purge()" << endl;
#endif

    if (noEntries != 0)
    {
        CrashRecRecord *temp;
        temp = listHead;
        while (temp != 0)
        {
            if ( temp->getRecoveryStatus() == COMPLETE )
                remove(temp);

            temp = temp->next;
        }
    }

    return(TRUE);
}

ostream& operator<< (ostream& strm, CRRecordList l)
{
    l.print(strm);
    return strm;
}
