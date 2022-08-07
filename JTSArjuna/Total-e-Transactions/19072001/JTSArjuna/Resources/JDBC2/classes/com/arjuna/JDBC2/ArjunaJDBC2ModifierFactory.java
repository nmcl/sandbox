/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2ModifierFactory.java,v 1.1.2.1.14.1.2.1.10.1.30.1 2001/07/19 14:16:07 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.jta.JTA_XAModifier;
import com.arjuna.JDBC2.drivers.modifiers.*;
import java.util.*;

/**
 * Keep track of any "modifiers" we may require to run
 * transactions over specific databases.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2ModifierFactory.java,v 1.1.2.1.14.1.2.1.10.1.30.1 2001/07/19 14:16:07 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2ModifierFactory
{

public static synchronized void putModifier (String dbName, JTA_XAModifier mod)
    {
	_modifiers.put(dbName, mod);
    }

    /*
     * Convert input to lower case first.
     */
    
public static synchronized JTA_XAModifier getModifier (String dbName)
    {
	Enumeration e = _modifiers.keys();
	
	dbName = dbName.toLowerCase();

	while (e.hasMoreElements())
	{
	    String s = (String) e.nextElement();

	    if (dbName.indexOf(s) != -1)
		return (JTA_XAModifier) _modifiers.get(s);
	}
	
	return null;
    }    

private static Hashtable _modifiers = new Hashtable();

    static
    {
	Object o = new com.arjuna.JDBC2.drivers.modifiers.list();
    }
    
}
