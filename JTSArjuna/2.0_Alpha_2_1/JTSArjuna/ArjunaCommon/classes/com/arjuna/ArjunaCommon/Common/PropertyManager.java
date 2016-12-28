/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PropertyManager.java,v 1.2.4.1 2000/04/25 08:35:55 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.util.*;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.SecurityException;

public class PropertyManager
{

public static synchronized void addPropertiesFile (String fileName)
    {
	propertyFiles.put(fileName, fileName);
    }

public static synchronized void removePropertiesFile (String fileName) throws FileNotFoundException
    {
	if (propertyFiles.remove(fileName) == null)
	    throw new FileNotFoundException();
    }
		
    /**
     * Read in the properties file, if present.
     */
    
public static synchronized void loadProperties () throws SecurityException, IOException
    {
	Enumeration propFiles = propertyFiles.elements();

	if (propFiles != null)
	{
	    Properties props = new Properties(System.getProperties());
	    
	    while (propFiles.hasMoreElements())
	    {
		String f = (String) propFiles.nextElement();

		try
		{		
		    props.load(new BufferedInputStream(new FileInputStream(f)));
	    
		    System.setProperties(props);
		}
		catch (FileNotFoundException e1)
		{
		}
	    }
	}
    }
    
public static synchronized void setProperty (String name, String property)
    {
	String oldProperty = (String) properties.get(name);

	if (oldProperty != null)
	    System.err.println("WARNING: PropertyManager.setProperty - overriding property <"+name+", "+oldProperty+"> with <"+name+", "+property+">");

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
private static Hashtable propertyFiles = new Hashtable();

    static
    {
	String propertyName = "Arjuna.properties";
	    
	/*
	 * See if the user has overridden the properties file.
	 */
	    
	String properties = System.getProperty(com.arjuna.ArjunaCommon.Common.Environment.PROPERTIES_FILE);

	/*
	 * If no override, then look locally before trying the
	 * global (install) directory.
	 */

	if (properties == null)
	{
	    String dir = System.getProperty("user.dir");  // assume it's set!
	    
	    properties = dir+File.separatorChar+propertyName;

	    try
	    {
		File localProps = new File(properties);

		if (!localProps.exists())
		{
		    /*
		     * Now try in the user's home directory.
		     */

		    dir = System.getProperty("user.home");  // assume it's set!

		    properties = dir+File.separatorChar+propertyName;

		    localProps = new File(properties);

		    if (!localProps.exists())
			properties = null;
		}

		localProps = null;
	    }
	    catch (Exception e)
	    {
		properties = null;
	    }
	}

	if (properties == null)
	    properties = com.arjuna.ArjunaCommon.Common.Configuration.propertiesFile();
		
	addPropertiesFile(properties);

	setProperty(com.arjuna.ArjunaCommon.Common.Environment.PROPERTIES_FILE, properties);
	
	try
	{
	    loadProperties();
	}
	catch (Exception e)
	{
	    System.err.println("PropertyManager initialiser failed: "+e);
	    System.exit(0);
	}
    }
	
};
