/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Arjuna.java,v 1.3 1999/02/17 10:18:57 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.ArjunaOTS;

/* 
 * To get the formatID used to represent Arjuna transactions
 * to the system.
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

public static int restrictedXID ()
    {
	return 131074;
    }
    
public static String arjunaXID ()
    {
	return "ArjunaXID";
    }

public static String arjunaStrictXID ()
    {
	return "ArjunaStrictXID";
    }
  
public static String arjunaRestrictedXID ()
    {
	return "ArjunaRestrictedXID";
    }

public static int nameToXID (String name)
    {
	if (name == null)
	    return -1;
	else
	{
	    if (name.compareTo(Arjuna.arjunaXID()) == 0)
		return Arjuna.XID();
	    else
	    {
		if (name.compareTo(Arjuna.arjunaStrictXID()) == 0)
		    return Arjuna.strictXID();
		else
		{
		    if (name.compareTo(Arjuna.arjunaRestrictedXID()) == 0)
			return Arjuna.restrictedXID();
		    else
			return -1;
		}
	    }
	}
    }
 
};
