/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PropertyManager.java,v 1.2.4.1.6.2.20.1.2.4.2.1.2.3.4.12.6.1.2.3.16.2 2001/06/15 12:04:02 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import com.arjuna.ArjunaCommon.Common.implementations.PropertyService.*;
import java.util.*;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.SecurityException;
import java.lang.ExceptionInInitializerError;

/**
 * The PropertyManager class provides a uniform interface to
 * accessing and manipulating Java properties. Property files
 * can be added to the PropertyManager and their contents will
 * automatically be loaded and added to the property address space.
 * Properties can also be added to the local address space.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: PropertyManager.java,v 1.2.4.1.6.2.20.1.2.4.2.1.2.3.4.12.6.1.2.3.16.2 2001/06/15 12:04:02 nmcl Exp $
 * @since JTS 1.0.
 */

public class PropertyManager
{

    /**
     * Add the specified property file to the list of registered
     * property files. This is always considered a relative path.
     */

public static synchronized void addPropertiesFile (String fileName)
    {
	createPropertyImple();
	
	_propertyImple.addPropertiesFile(fileName);
    }

    /**
     * Remove the specified property file from the list of registered
     * property files. If the file was not added, then the
     * FileNotFoundException will be thrown.
     */

public static synchronized void removePropertiesFile (String fileName) throws FileNotFoundException
    {
	if (_propertyImple == null)
	    throw new FileNotFoundException();
	else
	    _propertyImple.removePropertiesFile(fileName);
    }

    /**
     * @return the list of properties currently registered with the system.
     *
     * @since 2.1
     */		

public static synchronized Properties getProperties ()
    {
	return System.getProperties();
    }

    /**
     * Read in the properties file, if present. If the properties
     * have already been read in the nothing happens.
     */

public static void loadProperties () throws SecurityException, IOException
    {
	loadProperties(false);
    }
    
    /**
     * Read in the properties file, if present.
     *
     * @param force if this is <code>true</code> the property files will
     * be resourced, i.e., properties will be re-loaded.
     *
     * @since JTS 2.1.
     */

public static synchronized void loadProperties (boolean force) throws SecurityException, IOException
    {
	if (_loaded && !force)
	    return;

	createPropertyImple();
	
	_propertyImple.loadProperties();

	_loaded = true;
    }

    /**
     * Set the property name to have the specified value.
     * Flush the property value into the System
     * so that a call to System.getProperty will succeed.
     * Do not issue a warning message is an existing property value is changed.
     *
     * @since JTS 2.1.
     */

public static void setProperty (String name, String property)
    {
	setProperty(name, property, true, false);
    }

    /**
     * Set the property name to have the specified value.
     * If flush is true then we put the property value into the System
     * so that a call to System.getProperty will succeed. Otherwise we
     * cache it as a "volatile" value.
     * Issue a warning message is an existing property value is changed.
     *
     * @since JTS 2.1.
     */

public static void setProperty (String name, String property, boolean flush)
    {
	setProperty(name, property, flush, true);
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
    
public static synchronized void setProperty (String name, String property, boolean flush, boolean warn)
    {
	createPropertyImple();
	
	_propertyImple.setProperty(name, property, flush, warn);
    }

    /**
     * Return the value associated with the property name. Returns
     * null if the name does not exist.
     */

public static synchronized String getProperty (String name)
    {
	return getProperty(name, null);  // null also means we will check every time
    }

    /**
     * Return the value associated with the property name. If the name
     * does not exist then the specified default value will be returned.
     */

public static synchronized String getProperty (String name, String defaultValue)
    {
	createPropertyImple();
	
	return _propertyImple.getProperty(name, defaultValue);
    }

    /**
     * Remove the property.
     */

public static synchronized String removeProperty (String name)
    {
	createPropertyImple();
	
	return _propertyImple.removeProperty(name);
    }

    /**
     * Return a list of registered property names.
     *
     * @since JTS 2.1.
     */

public static synchronized Enumeration propertyNames ()
    {
	return System.getProperties().propertyNames();
    }

   /**
     * Output <name, value> pairs on System.out
     *
     * @since JTS 2.1.
     */

public static final synchronized void dumpProperties ()
    {
	Enumeration e = PropertyManager.propertyNames();
	
	while (e.hasMoreElements())
	{
	    String name = (String) e.nextElement();
	    String val = PropertyManager.getProperty(name);
	    
	    System.out.println(name+"="+val);
	}
    }

public static final boolean verbose ()
    {
	return _verbose;
    }

private static final void createPropertyImple ()
    {
	if (_propertyImple == null)
	{
	    String imple = System.getProperty(Environment.PROPERTY_MANAGER_IMPLEMENTATION, defaultImplementation);
	
	    if (imple == null)
	    {
		throw new FatalError("PropertyManager could not create a property service implementation!");
	    }
	    else
	    {
		try
		{
		    Class c = Class.forName(imple);

		    _propertyImple = (PropertyServiceImple) c.newInstance();
		}
		catch (Exception ex)
		{
		    throw new FatalError(ex.toString());
		}
	    }
	}
    }
	
private static PropertyServiceImple _propertyImple = null;
private static boolean              _verbose = false;
private static boolean              _loaded = false;
    
private static final String defaultImplementation = com.arjuna.ArjunaCommon.Common.implementations.PropertyService.PropertyFileImple.class.getName();
    
    static
    {
	if (!PropertyManager._loaded)
	{
	    /*
	     * We have to check for verbose-ness using the System properties
	     * otherwise we have a bootstrap problem!
	     */

	    String v = System.getProperty(Environment.VERBOSE_PROPERTY_MANAGER);
	    
	    if (v != null)
	    {
		if (v.equals("ON"))
		    PropertyManager._verbose = true;
	    }

	    /*
	     * See if the user has overridden the properties file.
	     */
	    
	    String defaultPropertyFile = System.getProperty(com.arjuna.ArjunaCommon.Common.Environment.PROPERTIES_FILE);

	    if (defaultPropertyFile == null)
		defaultPropertyFile = Configuration.propertiesFile();

	    PropertyManager.addPropertiesFile(defaultPropertyFile);
	    
	    try
	    {
		PropertyManager.loadProperties(true);
	    }
	    catch (Exception e)
	    {
		throw new ExceptionInInitializerError(e.toString());
	    }

	    if (!PropertyManager._loaded)
		throw new ExceptionInInitializerError("Could not initialise PropertyManager.");
	}
    }
	
}

