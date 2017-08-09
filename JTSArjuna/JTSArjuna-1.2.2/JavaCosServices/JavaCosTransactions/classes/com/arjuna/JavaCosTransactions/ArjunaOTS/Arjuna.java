/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Arjuna.java,v 1.2 1998/07/06 13:28:46 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.ArjunaOTS;

/* 
 * To get the formatID used to represent Arjuna transactions
 * to the system.
 * 
 * Do it this way rather than a #define or const so that we
 * can change the value without requiring recompilation of
 * the entire system.
 */

public class Arjuna
{
    
public static int XID ()
    {
	return 131072;
    }

public static int strictXID ()
    {
	return 131073;
    }
    
};

