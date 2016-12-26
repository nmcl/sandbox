/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: FacilityCode.java,v 1.1.8.1.8.1.16.1.4.3.2.1.32.1.22.1 2001/10/31 12:07:04 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

/**
 * The various levels of facility codes that are available.
 * Typically one for each major component in the system.
 *
 * <ul>
 * <li>FAC_ATOMIC_ACTION = 0x0000001 (atomic action core module).
 * <li>FAC_CONCURRENCY_CONTROL = 0x0000002 (concurrency control classes).
 * <li>FAC_BUFFER_MAN = 0x00000004 (state management (buffer) classes).
 * <li>FAC_ABSTRACT_REC = 0x00000008 (abstract records).
 * <li>FAC_OBJECT_STORE = 0x00000010 (object store implementations).
 * <li>FAC_STATE_MAN = 0x00000020 (state management and StateManager).
 * <li>FAC_SHMEM = 0x00000040 (shared memory implementation classes).
 * <li>FAC_GENERAL = 0x00000080 (general classes).
 * <li>FAC_OTS = 0x00000100 (OTS implementation classes).
 * <li>FAC_POS = 0x00000200 (POS implementation classes).
 * <li>FAC_OCCS = 0x00000400 (OCCS implementation classes).
 * <li>FAC_CRASH_RECOVERY = 0x00000800 (detailed trace of crash recovery module and classes).
 * <li>FAC_ORB_PORTABILITY = 0x00001000 (ORB Portability classes).
 * <li>FAC_THREADING = 0x00002000 (threading classes).
 * <li>FAC_FILTER = 0x00004000 (filter/interceptor classes).
 * <li>FAC_JDBC = 0x00008000 (JDBC 1.0 and 2.0 support).
 * <li>FAC_LOCK_STORE = 0x00010000 (lock store implementations).
 * <li>FAC_JTA = 0x00020000 (JTA support).
 * <li>FAC_RECOVERY_NORMAL = 0x00040000 (normal output for crash recovery manager).
 * <li>FAC_ALL = 0xffffffff (all classes).
 * </ul>
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FacilityCode.java,v 1.1.8.1.8.1.16.1.4.3.2.1.32.1.22.1 2001/10/31 12:07:04 nmcl Exp $
 * @since JTS 1.2.4.
 */
 
public class FacilityCode
{

public static final long FAC_ATOMIC_ACTION = 0x00000001;
public static final long FAC_CONCURRENCY_CONTROL = 0x00000002;
public static final long FAC_BUFFER_MAN = 0x00000004;
public static final long FAC_ABSTRACT_REC = 0x0000008;
public static final long FAC_OBJECT_STORE = 0x00000010;
public static final long FAC_STATE_MAN = 0x00000020;
public static final long FAC_SHMEM = 0x00000040;
public static final long FAC_GENERAL = 0x00000080;
public static final long FAC_OTS = 0x00000100;
public static final long FAC_POS = 0x00000200;
public static final long FAC_OCCS = 0x00000400;
public static final long FAC_CRASH_RECOVERY = 0x00000800;
public static final long FAC_ORB_PORTABILITY = 0x00001000;
public static final long FAC_THREADING = 0x00002000;
public static final long FAC_FILTER = 0x00004000;
public static final long FAC_JDBC = 0x00008000;
public static final long FAC_LOCK_STORE = 0x00010000;
public static final long FAC_JTA = 0x00020000;
public static final long FAC_RECOVERY_NORMAL = 0x00040000;
public static final long FAC_RESERVED1 = 0x00100000;
public static final long FAC_RESERVED2 = 0x00200000;
public static final long FAC_RESERVED3 = 0x00400000;
public static final long FAC_RESERVED4 = 0x00800000;
public static final long FAC_USER1 = 0x10000000;
public static final long FAC_USER2 = 0x20000000;
public static final long FAC_USER3 = 0x40000000;
public static final long FAC_USER4 = 0x80000000;
public static final long FAC_ALL = 0xfffffff;

    /**
     * @since JTS 2.1.2.
     */

public static final long getLevel (String level)
    {
	if (level.equals("FAC_ATOMIC_ACTION"))
	    return FAC_ATOMIC_ACTION;
	if (level.equals("FAC_CONCURRENCY_CONTROL"))
	    return FAC_CONCURRENCY_CONTROL;
	if (level.equals("FAC_BUFFER_MAN"))
	    return FAC_BUFFER_MAN;
	if (level.equals("FAC_ABSTRACT_REC"))
	    return FAC_ABSTRACT_REC;
	if (level.equals("FAC_OBJECT_STORE"))
	    return FAC_OBJECT_STORE;
	if (level.equals("FAC_STATE_MAN"))
	    return FAC_STATE_MAN;
	if (level.equals("FAC_SHMEM"))
	    return FAC_SHMEM;
	if (level.equals("FAC_GENERAL"))
	    return FAC_GENERAL;
	if (level.equals("FAC_OTS"))
	    return FAC_OTS;
	if (level.equals("FAC_POS"))
	    return FAC_POS;
	if (level.equals("FAC_OCCS"))
	    return FAC_OCCS;
	if (level.equals("FAC_CRASH_RECOVERY"))
	    return FAC_CRASH_RECOVERY;
	if (level.equals("FAC_ORB_PORTABILITY"))
	    return FAC_ORB_PORTABILITY;
	if (level.equals("FAC_THREADING"))
	    return FAC_THREADING;
	if (level.equals("FAC_FILTER"))
	    return FAC_FILTER;
	if (level.equals("FAC_JDBC"))
	    return FAC_JDBC;
	if (level.equals("FAC_LOCK_STORE"))
	    return FAC_LOCK_STORE;
	if (level.equals("FAC_JTA"))
	    return FAC_JTA;
	if (level.equals("FAC_RECOVERY_NORMAL"))
	    return FAC_RECOVERY_NORMAL;
	if (level.equals("FAC_RESERVED1"))
	    return FAC_RESERVED1;
	if (level.equals("FAC_RESERVED2"))
	    return FAC_RESERVED2;
	if (level.equals("FAC_RESERVED3"))
	    return FAC_RESERVED3;
	if (level.equals("FAC_RESERVED4"))
	    return FAC_RESERVED4;
	if (level.equals("FAC_USER1"))
	    return FAC_USER1;
	if (level.equals("FAC_USER2"))
	    return FAC_USER2;
	if (level.equals("FAC_USER3"))
	    return FAC_USER3;
	if (level.equals("FAC_USER4"))
	    return FAC_USER4;
	if (level.equals("FAC_ALL"))
	    return FAC_ALL;

	return FAC_USER1;
    }

    /**
     * @since JTS 2.1.2.
     */

public static final String printString (long level)
    {
	if (level == FAC_ATOMIC_ACTION)
	    return "FAC_ATOMIC_ACTION";
	if (level == FAC_CONCURRENCY_CONTROL)
	    return "FAC_CONCURRENCY_CONTROL";
	if (level == FAC_BUFFER_MAN)
	    return "FAC_BUFFER_MAN";
	if (level == FAC_ABSTRACT_REC)
	    return "FAC_ABSTRACT_REC";
	if (level == FAC_OBJECT_STORE)
	    return "FAC_OBJECT_STORE";
	if (level == FAC_STATE_MAN)
	    return "FAC_STATE_MAN";
	if (level == FAC_SHMEM)
	    return "FAC_SHMEM";
	if (level == FAC_ALL)
	    return "FAC_ALL";
	if (level == FAC_GENERAL)
	    return "FAC_GENERAL";
	if (level == FAC_OTS)
	    return "FAC_OTS";
	if (level == FAC_POS)
	    return "FAC_POS";
	if (level == FAC_OCCS)
	    return "FAC_OCCS";
	if (level == FAC_CRASH_RECOVERY)
	    return "FAC_CRASH_RECOVERY";
	if (level == FAC_ORB_PORTABILITY)
	    return "FAC_ORB_PORTABILITY";
	if (level == FAC_THREADING)
	    return "FAC_THREADING";
	if (level == FAC_FILTER)
	    return "FAC_FILTER";
	if (level == FAC_JDBC)
	    return "FAC_JDBC";
	if (level == FAC_LOCK_STORE)
	    return "FAC_LOCK_STORE";
	if (level == FAC_JTA)
	    return "FAC_JTA";
	if (level == FAC_RECOVERY_NORMAL)
	    return "FAC_RECOVERY_NORMAL";
	if (level == FAC_RESERVED1)
	    return "FAC_RESERVED1";
	if (level == FAC_RESERVED2)
	    return "FAC_RESERVED2";
	if (level == FAC_RESERVED3)
	    return "FAC_RESERVED3";
	if (level == FAC_RESERVED4)
	    return "FAC_RESERVED4";
	if (level == FAC_USER1)
	    return "FAC_USER1";
	if (level == FAC_USER2)
	    return "FAC_USER2";
	if (level == FAC_USER3)
	    return "FAC_USER3";
	if (level == FAC_USER4)
	    return "FAC_USER4";

	return "FAC_USER1";
    }
		
}

