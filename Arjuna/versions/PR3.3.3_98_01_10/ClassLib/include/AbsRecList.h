/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbsRecList.h,v 1.1 1993/12/14 11:28:45 nmcl Exp $
 */


#ifndef ABSRECLIST_H_
#define ABSRECLIST_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif


class AbstractRecord;

/*
 * This class maintains the dynamic linked-list of abstract record types.
 * Mainly used for crash recovery.
 */

class AbstractRecordList
{
public:
    AbstractRecordList (AbstractRecord* (*func1)(), void (*func2)(AbstractRecord*&), const TypeName);
    ~AbstractRecordList();
    
    AbstractRecord* create ();
    void remove (AbstractRecord*&);

    const TypeName type () const;

    AbstractRecordList* next;

private:
    AbstractRecord* (*RecordCreate)();
    void (*RecordDestroy)(AbstractRecord*&);
    TypeName tName;
};

#endif
