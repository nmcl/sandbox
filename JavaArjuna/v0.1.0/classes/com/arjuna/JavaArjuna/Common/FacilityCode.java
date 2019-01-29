/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: FacilityCode.java,v 1.1 2000/03/27 14:27:22 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

/* 
 * Enumerated types for FacilityCodes
 */
 
public class FacilityCode
{

public static final int FAC_ATOMIC_ACTION = 0x0001;
public static final int FAC_CONCURRENCY_CONTROL = 0x0002;
public static final int FAC_BUFFER_MAN = 0x0004;
public static final int FAC_ABSTRACT_REC = 0x008;
public static final int FAC_OBJECT_STORE = 0x0010;
public static final int FAC_STATE_MAN = 0x0020;
public static final int FAC_SHMEM = 0x0040;
public static final int FAC_GENERAL = 0x0080;
public static final int FAC_OTS = 0x0100;
public static final int FAC_POS = 0x0200;
public static final int FAC_OCCS = 0x0400;
public static final int FAC_CRASH_RECOVERY = 0x0800;
public static final int FAC_RESERVED1 = 0x1000;
public static final int FAC_RESERVED2 = 0x2000;
public static final int FAC_USER1 = 0x4000;
public static final int FAC_USER2 = 0x8000;
public static final int FAC_ALL = 0xffff;
    
};
