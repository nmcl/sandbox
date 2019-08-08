/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PropertyFileImple.java,v 1.1.2.2.2.1.20.2 2001/11/27 13:38:11 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common.implementations.PropertyService;

import com.arjuna.ArjunaCommon.Common.Configuration;
import com.arjuna.ArjunaCommon.Common.Environment;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.Utility;
import com.arjuna.ArjunaCommon.Common.ClassPathParser;
import java.util.*;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.SecurityException;
import java.lang.ExceptionInInitializerError;

/**
 * The PropertyFileImple class provides a uniform interface to
 * accessing and manipulating Java properties. Property files
 * can be added to the PropertyFileImple and their contents will
 * automatically be loaded and added to the property address space.
 * Properties can also be added to the local address space.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: PropertyFileImple.java,v 1.1.2.2.2.1.20.2 2001/11/27 13:38:11 nmcl Exp $
 * @since JTS 1.0.
 */

public class PropertyFileImple implements PropertyServiceImple
{

    /**
     * Add the specified property file to the list of registered
     * property files. This is always considered a relative path.
     */

public synchronized void addPropertiesFile (String fileName)
    {
	if (!propertyFiles.contains(fileName))
	    propertyFiles.add(fileName);
    }

    /**
     * Remove the specified property file from the list of registered
     * property files. If the file was not added, then the
     * FileNotFoundException will be thrown.
     */

public synchronized void removePropertiesFile (String fileName) throws FileNotFoundException
    {
	if (!propertyFiles.remove(fileName))
	    throw new FileNotFoundException();
    }
	
    /**
     * Read in the properties file, if present.
     *
     * @param force if this is <code>true</code> the property files will
     * be resourced, i.e., properties will be re-loaded.
     *
     * @since JTS 2.1.
     */

public synchronized void loadProperties () throws SecurityException, IOException
    {
	// get rid of old properties

	properties = new Hashtable();
	
	Properties props = System.getProperties();

	for (int i = 0; i < propertyFiles.size(); i++)
	{
	    String f = (String) propertyFiles.elementAt(i);

	    try
	    {
		String fullPath = locateFile(f);

		FileInputStream fs = new FileInputStream(fullPath);
		BufferedInputStream bs = new BufferedInputStream(fs);
		    
		props.load(bs);
	    
		fs.close();
		bs.close();
	    }
	    catch (FileNotFoundException e1)
	    {
		if (PropertyManager.verbose())
		    ErrorStream.warning().println("PropertyFileImple could not locate property file: "+f);
	    }
	}

	if (propertyFiles.size() > 0)
	    System.setProperties(props);

	if (PropertyManager.verbose())
	{
	    System.out.println("\nProperties loaded:\n");
	    
	    PropertyManager.dumpProperties();
	}
    }

    /**
     * Set the property name to have the specified value.
     * If flush is true then we put the property value into the System
     * so that a call to System.getProperty will succeed. Otherwise we
     * cache it as a "volatile" value.
     * If <code>warn</code> is <code>true</code> then issue a warning
     * message if an existing property value is changed.
     *
     * @since JTS 2.1.
     */
    
public synchronized void setProperty (String name, String property, boolean flush, boolean warn)
    {
	String oldProperty = (String) properties.get(name);

	if ((oldProperty != null) && (warn))
	{
	    ErrorStream.warning().println("PropertyFileImple.setProperty - overriding property <"+name+", "+oldProperty+"> with <"+name+", "+property+">");
	}

	properties.put(name, property);

	if (flush)
	    System.setProperty(name, property);
    }

    /**
     * @return the value associated with the property name. If the name
     * does not exist then the specified default value will be returned.
     */

public synchronized String getProperty (String name, String defaultValue)
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

    /**
     * Remove the property.
     */

public synchronized String removeProperty (String name)
    {
	return (String) properties.remove(name);
    }

    /*
     * Locate the specified property file.
     */
    
private static final synchronized String locateFile (String propertyName) throws FileNotFoundException
    {
	String fullPathName = null;
	String dir = null;

	if (propertyName != null)
	{
	    if (propertyName.startsWith(absolutePath))  // relative path?
	    {
		fullPathName = propertyName.substring(absolutePath.length());
	    }
	}
	
	if ((fullPathName == null) && (propertyName != null))
	{
	    if (PropertyManager.verbose())
		System.out.println("PropertyFileImple searching in user.dir for: "+propertyName);
	    
	    dir = System.getProperty("user.dir");  // assume it's set!

	    fullPathName = dir + File.separatorChar + propertyName;

	    try
	    {
		File localProps = new File(fullPathName);

		if (!localProps.exists())
		{
		    if (PropertyManager.verbose())
			System.out.println("PropertyFileImple searching in user.home for: "+propertyName);

		    /*
		     * Now try in the user's home directory.
		     */

		    dir = System.getProperty("user.home");  // assume it's set!

		    if (dir != null)
		    {
			fullPathName = dir + File.separatorChar + propertyName;

			localProps = new File(fullPathName);
		    }
		    else
			localProps = null;

		    if ((localProps != null) && (!localProps.exists()))
		    {
			if (PropertyManager.verbose())
			    System.out.println("PropertyFileImple searching in java.home for: "+propertyName);

			dir = System.getProperty("java.home");

			if (dir != null)
			{
			    fullPathName = dir + File.separatorChar + "apps" + File.separatorChar + "Arjuna" + File.separatorChar + propertyName;

			    localProps = new File(fullPathName);
			}
			else
			    localProps = null;

			if ((localProps != null) && !localProps.exists())
			{
			    fullPathName = null;
			    
			    if (PropertyManager.verbose())
				System.out.println("PropertyFileImple searching CLASSPATH");

			    ClassPathParser cp = new ClassPathParser();

			    dir = cp.getPath(File.separator + "etc");

			    if (dir == null)
			    {
				if (Utility.isWindows())  // cygwin issue
				    dir = cp.getPath("/etc");
			    }

			    while (dir != null)
			    {
				fullPathName = dir + File.separatorChar + propertyName;

				localProps = new File(fullPathName);

				if (localProps.exists())
				    break;
				else
				    fullPathName = null;

				dir = cp.getPath(File.separator + "etc");

				if (dir == null)
				{
				    if (Utility.isWindows())
					dir = cp.getPath("/etc");
				}
			    }
			    
			    if (fullPathName == null)
			    {
				if (PropertyManager.verbose())
				    System.out.println("PropertyFileImple search failed in "+dir);
			    }
			    else
			    {
				if (PropertyManager.verbose())
				    System.out.println("PropertyFileImple search failed in CLASSPATH");
			    }
			}
			else
			{
			    if (PropertyManager.verbose())
				System.out.println("PropertyFileImple found in java.home");
			}
		    }
		    else
		    {
			if (PropertyManager.verbose())
			    System.out.println("PropertyFileImple found in user.home");
		    }
		}
		else
		{
		    if (PropertyManager.verbose())
			System.out.println("PropertyFileImple found in user.dir");
		}

		localProps = null;
	    }
	    catch (Exception e)
	    {
		fullPathName = null;
	    }
	}
	else
	    throw new FileNotFoundException("PropertyFileImple - null property file name!");

	if (fullPathName == null)
	    throw new FileNotFoundException("PropertyFileImple could not find: "+propertyName);

	/*
	 * If this is the core property file then store the location.
	 */

	if (propertyName.equals(Configuration.propertiesFile()) && (dir != null))
	{
	    Configuration.setPropertiesDir(dir);
	}
	
	if (PropertyManager.verbose())
	{
	    System.out.println("PropertyFileImple returning: "+fullPathName);
	}

	return fullPathName;
    }

private static Hashtable properties = new Hashtable();  // local cache
private static Vector    propertyFiles = new Vector();

private static final String absolutePath = "abs://";
	
}

