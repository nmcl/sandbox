/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBInitLoader.java,v 1.1 2000/02/25 14:09:19 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.util.Properties;
import java.util.Enumeration;
import java.lang.ClassLoader;

import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.lang.InstantiationException;

/*
 * This class allows the programmer (and us!) to register classes
 * which must be instantiated before/after ORB initialisation.
 */

abstract class ORBInitLoader
{

protected ORBInitLoader (String name, String attrName)
    {
	initName = name;
	propertyName = attrName;
    }

protected void initialise ()
    {
	Properties properties = System.getProperties();

	if (properties != null)
	{
	    Enumeration names = properties.propertyNames();
	    
	    while (names.hasMoreElements())
	    {
		String attrName = (String) names.nextElement();

		if (attrName.indexOf(propertyName) != -1)
		    createInstance(attrName, properties.getProperty(attrName));
	    }
	}
    }

private void createInstance (String attrName, String className)
    {
	if (className == null)
	{
	    System.err.println(initName+" warning - attempt to initialise "+attrName+" with null class name!");
	    return;
	}
	else
	{
	    try
	    {
		Class c = classLoader.loadClass(className);

		try
		{
		    Object o = c.newInstance();
		    o = null;
		}
		catch (IllegalAccessException e1)
		{
		    System.err.println(initName+" "+e1);
		}
		catch (InstantiationException e2)
		{
		    System.err.println(initName+" "+e2);
		}

		c = null;
	    }
	    catch (ClassNotFoundException e)
	    {
		System.err.println(initName+" warning - could not find class "+className);
	    }
	}
    }

private String initName;
private String propertyName;
    
private static final ORBClassLoader classLoader = new ORBClassLoader();

};
