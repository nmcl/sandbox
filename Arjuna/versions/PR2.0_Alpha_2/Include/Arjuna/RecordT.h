/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordT.h,v 1.1 1993/11/03 14:27:59 nmcl Exp $
 */

#ifndef _RECORDT_H_
#define _RECORDT_H_ 

/*
 *
 * Object Record Types
 *
 */

/*
 * The following enumerated type defines the types of record that are
 * derived from AbstractRecord. The type also defines the order in which
 * these types may be held by an AtomicAction if a record refers to the
 * same object (as defined by the AbstractRecord operator== operation).
 * Since records are processed sequentially during an AtomicAction
 * operation the ordering below also defines the order in which each
 * operation is invoked.
 *
 * Hence a LOCK type record will always have operations invoked before a
 * RPCTERMINATE type record. This ordering is important otherwise some
 * records may negate the effects of other record, e.g. during the 
 * top_level_commit operation a RPCTERMINATE record terminates a 
 * server - this should not occur before the server record has sent the 
 * final Commit rpc.
 */

enum RecordType
{

    /* 
     * The following values are provided for user-defined
     * records that should have operations invoked BEFORE the
     * system records 
     */

    USER_DEF_FIRST0, USER_DEF_FIRST1, USER_DEF_FIRST2,
    USER_DEF_FIRST3, USER_DEF_FIRST4, USER_DEF_FIRST5,
    USER_DEF_FIRST6, USER_DEF_FIRST7, USER_DEF_FIRST8,
    USER_DEF_FIRST9,

    /* the values are used by the system records */
    
    RECOVERY, PERSISTENCE, LOCK, ACTIVATION, CADAVER,
    RPCCALL, RPCINITIATE,

    /* 
     * The following values are provided for user-defined
     * records that should have operations invoked AFTER the
     * system records
     */

    USER_DEF_LAST0, USER_DEF_LAST1, USER_DEF_LAST2,
    USER_DEF_LAST3, USER_DEF_LAST4, USER_DEF_LAST5,
    USER_DEF_LAST6, USER_DEF_LAST7, USER_DEF_LAST8,
    USER_DEF_LAST9,

    /* 
     * Note that RPCTERMINATE is deliberately late in the list
     * otherwise the server would be terminated BEFORE the user
     * records were processed.
     */

    RPCTERMINATE, RPCCADAVER,
    UNTYPED, NONE_RECORD
};

class ostream;

extern ostream& operator<<  ( ostream& strm, RecordType rt );

#endif
