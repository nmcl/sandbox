/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecordType.java,v 1.4 1998/07/06 13:26:30 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import java.io.PrintStream;

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

public class RecordType
{

    /* 
     * The following values are provided for user-defined
     * records that should have operations invoked BEFORE the
     * system records 
     */

public static final int USER_DEF_FIRST0 = 0;
public static final int USER_DEF_FIRST1 = 1;
public static final int USER_DEF_FIRST2 = 2;
public static final int USER_DEF_FIRST3 = 3;
public static final int USER_DEF_FIRST4 = 4;
public static final int USER_DEF_FIRST5 = 5;
public static final int USER_DEF_FIRST6 = 6;
public static final int USER_DEF_FIRST7 = 7;
public static final int USER_DEF_FIRST8 = 8;
public static final int USER_DEF_FIRST9 = 9;

    /* the values are used by the system records */

public static final int RECOVERY = 10;
public static final int PERSISTENCE = 11;
public static final int LOCK = 12;
public static final int ACTIVATION = 13;
public static final int REPLICATION = 14;
public static final int NAMING = 15;
public static final int CADAVER = 16;
public static final int RPCCALL = 17;
public static final int RPCINITIATE = 18;

    /* 
     * The following values are provided for user-defined
     * records that should have operations invoked AFTER the
     * system records
     */

public static final int USER_DEF_LAST0 = 19;
public static final int USER_DEF_LAST1 = 20;

public static final int USER_DEF_LAST2 = 21;
public static final int USER_DEF_LAST3 = 22;
public static final int USER_DEF_LAST4 = 23;
public static final int USER_DEF_LAST5 = 24;
public static final int USER_DEF_LAST6 = 25;
public static final int USER_DEF_LAST7 = 26;
public static final int USER_DEF_LAST8 = 27;
public static final int USER_DEF_LAST9 = 28;
    
    /* 
     * Note that RPCTERMINATE is deliberately late in the list
     * otherwise the server would be terminated BEFORE the user
     * records were processed.
     */

public static final int RPCTERMINATE = 29;
public static final int RPCCADAVER = 30;
public static final int UNTYPED = 31;
public static final int NONE_RECORD = 32;

public static void print (PrintStream strm, int rt)
    {
	 switch (rt)
	 {
	 case RecordType.USER_DEF_FIRST0:
	     strm.print("RecordType.USER_DEF_FIRST0");
	     break;
	 case RecordType.USER_DEF_FIRST1:
	     strm.print("RecordType.USER_DEF_FIRST1");
	     break;
	 case RecordType.USER_DEF_FIRST2:
	     strm.print("RecordType.USER_DEF_FIRST2");
	     break;
	 case RecordType.USER_DEF_FIRST3:
	     strm.print("RecordType.USER_DEF_FIRST3");
	     break;
	 case RecordType.USER_DEF_FIRST4:
	     strm.print("RecordType.USER_DEF_FIRST4");
	     break;
	 case RecordType.USER_DEF_FIRST5:
	     strm.print("RecordType.USER_DEF_FIRST5");
	     break;
	 case RecordType.USER_DEF_FIRST6:
	     strm.print("RecordType.USER_DEF_FIRST6");
	     break;
	 case RecordType.USER_DEF_FIRST7:
	     strm.print("RecordType.USER_DEF_FIRST7");
	     break;
	 case RecordType.USER_DEF_FIRST8:
	     strm.print("RecordType.USER_DEF_FIRST8");
	     break;
	 case RecordType.USER_DEF_FIRST9:
	     strm.print("RecordType.USER_DEF_FIRST9");
	     break;

	 case RecordType.LOCK:
	     strm.print("RecordType.LOCK");
	     break;
	 case RecordType.RECOVERY:
	     strm.print("RecordType.RECOVERY");
	     break;
	 case RecordType.PERSISTENCE:
	     strm.print("RecordType.PERSISTENCE");
	     break;
	 case RecordType.CADAVER:
	     strm.print("RecordType.CADAVER");
	     break;
	 case RecordType.ACTIVATION:
	     strm.print("RecordType.ACTIVATION");
	     break;
	 case RecordType.REPLICATION:
	     strm.print("RecordType.REPLICATION");
	     break;
	 case RecordType.NAMING:
	     strm.print("RecordType.NAMING");
	     break;
	 case RecordType.RPCCALL:
	     strm.print("RecordType.RPCCALL");
	     break;
	 case RecordType.RPCINITIATE:
	     strm.print("RecordType.RPCINITIATE");
	     break;
	 case RecordType.RPCTERMINATE:
	     strm.print("RecordType.RPCTERMINATE");
	     break;
	 case RecordType.RPCCADAVER:
	     strm.print("RecordType.RPCCADAVER");
	     break;

	 case RecordType.USER_DEF_LAST0:
	     strm.print("RecordType.USER_DEF_LAST0");
	     break;
	 case RecordType.USER_DEF_LAST1:
	     strm.print("RecordType.USER_DEF_LAST1");
	     break;
	 case RecordType.USER_DEF_LAST2:
	     strm.print("RecordType.USER_DEF_LAST2");
	     break;
	 case RecordType.USER_DEF_LAST3:
	     strm.print("RecordType.USER_DEF_LAST3");
	     break;
	 case RecordType.USER_DEF_LAST4:
	     strm.print("RecordType.USER_DEF_LAST4");
	     break;
	 case RecordType.USER_DEF_LAST5:
	     strm.print("RecordType.USER_DEF_LAST5");
	     break;
	 case RecordType.USER_DEF_LAST6:
	     strm.print("RecordType.USER_DEF_LAST6");
	     break;
	 case RecordType.USER_DEF_LAST7:
	     strm.print("RecordType.USER_DEF_LAST7");
	     break;
	 case RecordType.USER_DEF_LAST8:
	     strm.print("RecordType.USER_DEF_LAST8");
	     break;
	 case RecordType.USER_DEF_LAST9:
	     strm.print("RecordType.USER_DEF_LAST9");
	     break;

	 case RecordType.UNTYPED:
	     strm.print("RecordType.UNTYPED");
	     break;

	 case RecordType.NONE_RECORD:
	     strm.print("RecordType.NONE_RECORD");
	     break;

	 default:
	     strm.print("RecordType.** Unknown **");
	     break;
	 }
    }

};
