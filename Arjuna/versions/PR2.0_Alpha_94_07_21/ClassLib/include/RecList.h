/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecList.h,v 1.1 1993/11/03 12:29:07 nmcl Exp $
 */

#ifndef RECLIST_H_
#define RECLIST_H_

/*
 *
 * Record List Class. Manages instances of classes derived from
 * AbstractRecord as an ordered doubly-linked list. Normal insertion
 * is ordered, however functions exist to explicitly put elements
 * at the front and rear of the list, and to remove them from the
 * front and rear also.
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class ostream;
class AbstractRecord;

class RecordList
{
    friend class RecordListI;
    
public:
    /* Constructors and destructor */

    RecordList ();
    ~RecordList ();

    /* non-virtual member functions */

    AbstractRecord *getFront ();
    AbstractRecord *getRear ();
    Boolean insert (AbstractRecord *newRecord);

    ostream& print (ostream& strm) const;  
 
    void putFront (AbstractRecord *newRecord);
    void putRear (AbstractRecord *newRecord);
    int size () const;

private:
    /* non-virtual member functions */

    Boolean insert (AbstractRecord *newRecord, AbstractRecord *startAt);
    void insertBefore (AbstractRecord *newRecord, AbstractRecord *before);
    void replace (AbstractRecord *newRecord, AbstractRecord *oldRecord);    
    
    /* private state variables */
    
    AbstractRecord *listHead;
    AbstractRecord *listTail;
    int noEntries;
};

#include "RecList.n"

#endif
