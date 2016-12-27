/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowStatus.java,v 1.2 1998/07/06 10:49:46 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

/*
 * Enum: ShadowStatus
 * Records status of a shadow state managed by the associated ShadowRecord
 */

public class ShadowStatus
{
    
public static final int UNRESOLVED_STATE = 0;
public static final int COMMITTED_STATE = 1;
public static final int ABORTED_STATE = 2;
public static final int IGNORE_STATE = 3;
    
};
