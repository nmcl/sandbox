/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SAAStatus.java,v 1.2 1998/07/06 10:49:44 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

/*
 * Enum: Server Atomic Action Status (SAAStatus)
 * Status of a server stomic action record
 */

public class SAAStatus
{
    
public static final int UNRESOLVED_SAA = 0;
public static final int COMMIT_SAA = 1;
public static final int ABORT_SAA = 2;
public static final int IGNORE_SAA = 3;
public static final int COMMITTED_SAA = 4;
public static final int ABORTED_SAA = 5;

}
