/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecordType.java,v 1.1.4.1 2000/04/25 08:36:00 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.*;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaLite.ArjunaLiteNames;

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
     * system records.
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
public static final int DISPOSE = 17;
public static final int RPCCALL = 18;
public static final int RPCINITIATE = 19;

    /* 
     * The following values are provided for user-defined
     * records that should have operations invoked AFTER the
     * system records.
     */

public static final int USER_DEF_LAST0 = 20;
public static final int USER_DEF_LAST1 = 21;

public static final int USER_DEF_LAST2 = 22;
public static final int USER_DEF_LAST3 = 23;
public static final int USER_DEF_LAST4 = 24;
public static final int USER_DEF_LAST5 = 25;
public static final int USER_DEF_LAST6 = 26;
public static final int USER_DEF_LAST7 = 27;
public static final int USER_DEF_LAST8 = 28;
public static final int USER_DEF_LAST9 = 29;
    
    /* 
     * Note that RPCTERMINATE is deliberately late in the list
     * otherwise the server would be terminated BEFORE the user
     * records were processed.
     */

public static final int RPCTERMINATE = 30;
public static final int RPCCADAVER = 31;
public static final int UNTYPED = 32;
public static final int NONE_RECORD = 33;

public static ClassName typeToClassName (int rt)
    {
	 switch (rt)
	 {
	 case RecordType.USER_DEF_FIRST0:
	     return new ClassName("RecordType.USER_DEF_FIRST0");
	 case RecordType.USER_DEF_FIRST1:
	     return new ClassName("RecordType.USER_DEF_FIRST1");
	 case RecordType.USER_DEF_FIRST2:
	     return new ClassName("RecordType.USER_DEF_FIRST2");
	 case RecordType.USER_DEF_FIRST3:
	     return new ClassName("RecordType.USER_DEF_FIRST3");
	 case RecordType.USER_DEF_FIRST4:
	     return new ClassName("RecordType.USER_DEF_FIRST4");
	 case RecordType.USER_DEF_FIRST5:
	     return new ClassName("RecordType.USER_DEF_FIRST5");
	 case RecordType.USER_DEF_FIRST6:
	     return new ClassName("RecordType.USER_DEF_FIRST6");
	 case RecordType.USER_DEF_FIRST7:
	     return new ClassName("RecordType.USER_DEF_FIRST7");
	 case RecordType.USER_DEF_FIRST8:
	     return new ClassName("RecordType.USER_DEF_FIRST8");
	 case RecordType.USER_DEF_FIRST9:
	     return new ClassName("RecordType.USER_DEF_FIRST9");

	 case RecordType.LOCK:
	     return new ClassName("RecordType.LOCK");
	 case RecordType.RECOVERY:
	     return new ClassName("RecordType.RECOVERY");
	 case RecordType.PERSISTENCE:
	     return ArjunaLiteNames.Implementation_AbstractRecord_PersistenceRecord();
	 case RecordType.CADAVER:
	     return ArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord();
	 case RecordType.DISPOSE:
	     return ArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord_DisposeRecord();
	 case RecordType.ACTIVATION:
	     return new ClassName("RecordType.ACTIVATION");
	 case RecordType.REPLICATION:
	     return new ClassName("RecordType.REPLICATION");
	 case RecordType.NAMING:
	     return new ClassName("RecordType.NAMING");
	 case RecordType.RPCCALL:
	     return new ClassName("RecordType.RPCCALL");
	 case RecordType.RPCINITIATE:
	     return new ClassName("RecordType.RPCINITIATE");
	 case RecordType.RPCTERMINATE:
	     return new ClassName("RecordType.RPCTERMINATE");
	 case RecordType.RPCCADAVER:
	     return new ClassName("RecordType.RPCCADAVER");

	 case RecordType.USER_DEF_LAST0:
	     return new ClassName("RecordType.USER_DEF_LAST0");
	 case RecordType.USER_DEF_LAST1:
	     return new ClassName("RecordType.USER_DEF_LAST1");
	 case RecordType.USER_DEF_LAST2:
	     return new ClassName("RecordType.USER_DEF_LAST2");
	 case RecordType.USER_DEF_LAST3:
	     return new ClassName("RecordType.USER_DEF_LAST3");
	 case RecordType.USER_DEF_LAST4:
	     return new ClassName("RecordType.USER_DEF_LAST4");
	 case RecordType.USER_DEF_LAST5:
	     return new ClassName("RecordType.USER_DEF_LAST5");
	 case RecordType.USER_DEF_LAST6:
	     return new ClassName("RecordType.USER_DEF_LAST6");
	 case RecordType.USER_DEF_LAST7:
	     return new ClassName("RecordType.USER_DEF_LAST7");
	 case RecordType.USER_DEF_LAST8:
	     return new ClassName("RecordType.USER_DEF_LAST8");
	 case RecordType.USER_DEF_LAST9:
	     return new ClassName("RecordType.USER_DEF_LAST9");

	 case RecordType.UNTYPED:
	     return new ClassName("RecordType.UNTYPED");

	 case RecordType.NONE_RECORD:
	     return new ClassName("RecordType.NONE_RECORD");

	 default:
	     return null;
	 }
    }

public static int classNameToType (ClassName cn)
    {
	if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST0") == 0)
	    return USER_DEF_FIRST0;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST1") == 0)
		return USER_DEF_FIRST1;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST2") == 0)
		return USER_DEF_FIRST2;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST3") == 0)
		return USER_DEF_FIRST3;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST4") == 0)
		return USER_DEF_FIRST4;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST5") == 0)
		return USER_DEF_FIRST5;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST6") == 0)
		return USER_DEF_FIRST6;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST7") == 0)
		return USER_DEF_FIRST7;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST8") == 0)
		return USER_DEF_FIRST8;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_FIRST9") == 0)
		return USER_DEF_FIRST9;
	
	else
	    if (cn.stringForm().compareTo("RecordType.LOCK") == 0)
		return LOCK;
	else
	    if (cn.stringForm().compareTo("RecordType.RECOVERY") == 0)
		return RECOVERY;
	else
	    if (cn.equals(ArjunaLiteNames.Implementation_AbstractRecord_PersistenceRecord()))
		return PERSISTENCE;
	else
	    if (cn.equals(ArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord()))
		return CADAVER;
	else
	    if (cn.equals(ArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord_DisposeRecord()))
		return DISPOSE;
	else
	    if (cn.stringForm().compareTo("RecordType.ACTIVATION") == 0)
		return ACTIVATION;
	else
	    if (cn.stringForm().compareTo("RecordType.REPLICATION") == 0)
		return REPLICATION;
	else
	    if (cn.stringForm().compareTo("RecordType.NAMING") == 0)
		return NAMING;
	else
	    if (cn.stringForm().compareTo("RecordType.RPCCALL") == 0)
		return RPCCALL;
	else
	    if (cn.stringForm().compareTo("RecordType.RPCINITIATE") == 0)
		return RPCINITIATE;
	else
	    if (cn.stringForm().compareTo("RecordType.RPCTERMINATE") == 0)
		return RPCTERMINATE;
	else
	    if (cn.stringForm().compareTo("RecordType.RPCCADAVER") == 0)
		return RPCCADAVER;

	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST0") == 0)
		return USER_DEF_LAST0;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST1") == 0)
		return USER_DEF_LAST1;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST2") == 0)
		return USER_DEF_LAST2;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST3") == 0)
		return USER_DEF_LAST3;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST4") == 0)
		return USER_DEF_LAST4;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST5") == 0)
		return USER_DEF_LAST5;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST6") == 0)
		return USER_DEF_LAST6;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST7") == 0)
		return USER_DEF_LAST7;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST8") == 0)
		return USER_DEF_LAST8;
	else
	    if (cn.stringForm().compareTo("RecordType.USER_DEF_LAST9") == 0)
		return USER_DEF_LAST9;

	else
	    if (cn.stringForm().compareTo("RecordType.UNTYPED") == 0)
		return UNTYPED;

	else
	    if (cn.stringForm().compareTo("RecordType.NONE_RECORD") == 0)
		return NONE_RECORD;

	else
	    return -1;
    }
    
public static void print (PrintStream strm, int rt)
    {
	print(new PrintWriter(strm), rt);
    }

public static void print (PrintWriter strm, int rt)
    {
	strm.print(typeToClassName(rt));
	strm.flush();
    }

};
