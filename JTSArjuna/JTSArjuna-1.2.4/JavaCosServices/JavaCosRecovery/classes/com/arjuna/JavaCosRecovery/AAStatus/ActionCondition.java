/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionCondition.java,v 1.2 1998/07/06 10:49:48 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AAStatus;

/* 
 * Enum: ActionCondition
 * Records the status of an atomic action
 */

public class ActionCondition
{

public static final int ACTION_STATUS_UNKNOWN = 0;
public static final int ACTION_COMMITTED = 1;
public static final int ACTION_ABORTED = 2;
public static final int ACTION_ATTACHED = 3;

};
