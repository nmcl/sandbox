/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BrandingFactory.java,v 1.1 2003/06/19 10:50:31 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.jdbc.brandings;

import java.util.*;

/**
 * Keep track of any "brandings" we may require.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BrandingFactory.java,v 1.1 2003/06/19 10:50:31 nmcl Exp $
 * @since JTS 2.0.
 */

public class BrandingFactory
{

    public static synchronized void putBranding (String dbName, Brander brand)
    {
	_brandings.put(dbName, brand);
    }

    /*
     * Convert input to lower case first.
     */

    public static synchronized Brander getBranding (String dbName)
    {
	Enumeration e = _brandings.keys();

	dbName = dbName.toLowerCase();
	
	while (e.hasMoreElements())
	{
	    String s = (String) e.nextElement();
	
	    if (dbName.indexOf(s) != -1)
		return (Brander) _brandings.get(s);
	}
	
	return null;
    }

    private static Hashtable _brandings = new Hashtable();

    static
    {
	Object o = new com.arjuna.ats.internal.arjuna.jdbc.brandings.list();
    }
    
}
