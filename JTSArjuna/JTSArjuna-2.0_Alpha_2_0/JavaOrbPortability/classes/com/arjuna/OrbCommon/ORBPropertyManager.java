/*
 * Copyright (C) 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBPropertyManager.java,v 1.1.2.1 1999/01/28 19:14:00 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import com.arjuna.JavaGandiva.Common.Utility;

import java.util.Hashtable;

public class ORBPropertyManager
{

public static synchronized void setProperty (String name, String property)
    {
	String oldProperty = (String) properties.get(name);

	if (oldProperty != null)
	    System.err.println("WARNING: ORBPropertyManager.setProperty - overriding property <"+name+", "+oldProperty+"> with <"+name+", "+property+">");

	properties.put(name, property);
    }
    
public static synchronized String getProperty (String name)
    {
	return getProperty(name, null);  // null also means we will check every time
    }
    
public static synchronized String getProperty (String name, String defaultValue)
    {
	String property = (String) properties.get(name);

	if (property == null)
	{
	    property = System.getProperty(name, defaultValue);

	    /*
	     * Now cache value in case of subsequent checks.
	     */
	    
	    if (property != null)
		properties.put(name, property);
	}

	return property;
    }
    
private static Hashtable properties = new Hashtable();

    static
    {
	Utility.loadProperties();
    }
    
};
