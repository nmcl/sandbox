/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Arjuna.java,v 1.1.30.2.4.1 2000/12/21 11:21:19 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

/**
 * To get the formatID used to represent Arjuna transactions
 * to the system.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Arjuna.java,v 1.1.30.2.4.1 2000/12/21 11:21:19 nmcl Exp $
 * @since JTS 1.0.
 */

public class Arjuna
{
    
public static final int XID ()
    {
	return 131072;
    }

public static final int strictXID ()
    {
	return 131073;
    }

public static final int restrictedXID ()
    {
	return 131074;
    }
    
public static final String arjunaXID ()
    {
	return "ArjunaXID";
    }

public static final String arjunaStrictXID ()
    {
	return "ArjunaStrictXID";
    }
  
public static final String arjunaRestrictedXID ()
    {
	return "ArjunaRestrictedXID";
    }

public static final String osiXID ()
    {
	return "OSI";
    }
    
public static final int nameToXID (String name)
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
		    {
			if (name.compareTo(Arjuna.osiXID()) == 0)
			    return 0; // osi tp
			else
			    return -1;
		    }
		}
	    }
	}
    }
 
}
