/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ArjunaJDBC2BrandingFactory.java,v 1.1.2.1.16.1.2.1.2.1.10.1.30.1 2001/07/19 14:16:06 nmcl Exp $
 */

package com.arjuna.JDBC2;

import com.arjuna.JDBC2.drivers.brandings.*;
import java.util.*;

/**
 * Keep track of any "brandings" we may require.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ArjunaJDBC2BrandingFactory.java,v 1.1.2.1.16.1.2.1.2.1.10.1.30.1 2001/07/19 14:16:06 nmcl Exp $
 * @since JTS 2.0.
 */

public class ArjunaJDBC2BrandingFactory
{

public static synchronized void putBranding (String dbName, JDBC2Branding brand)
    {
	_brandings.put(dbName, brand);
    }

    /*
     * Convert input to lower case first.
     */

public static synchronized JDBC2Branding getBranding (String dbName)
    {
	Enumeration e = _brandings.keys();

	dbName = dbName.toLowerCase();
	
	while (e.hasMoreElements())
	{
	    String s = (String) e.nextElement();
	
	    if (dbName.indexOf(s) != -1)
		return (JDBC2Branding) _brandings.get(s);
	}
	
	return null;
    }

private static Hashtable _brandings = new Hashtable();

    static
    {
	Object o = new com.arjuna.JDBC2.drivers.brandings.list();
    }
    
}
