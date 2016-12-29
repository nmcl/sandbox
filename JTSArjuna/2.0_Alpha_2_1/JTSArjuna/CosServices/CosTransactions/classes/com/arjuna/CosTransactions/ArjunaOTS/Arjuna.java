/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Arjuna.java,v 1.1 2000/02/25 14:05:29 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

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
