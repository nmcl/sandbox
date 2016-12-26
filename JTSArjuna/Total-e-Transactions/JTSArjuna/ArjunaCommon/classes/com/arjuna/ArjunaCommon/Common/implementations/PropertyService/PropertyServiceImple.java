/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PropertyServiceImple.java,v 1.1.2.1 2001/05/25 13:22:21 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common.implementations.PropertyService;

import com.arjuna.ArjunaCommon.Common.*;
import java.util.*;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.SecurityException;
import java.lang.ExceptionInInitializerError;

/**
 * The PropertyServiceImple class provides a uniform interface to
 * accessing and manipulating Java properties.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: PropertyServiceImple.java,v 1.1.2.1 2001/05/25 13:22:21 nmcl Exp $
 * @since JTS 2.1.2.
 */

public interface PropertyServiceImple
{

    /**
     * Add the specified property file to the list of registered
     * property files.
     */

public void addPropertiesFile (String fileName);

    /**
     * Remove the specified property file from the list of registered
     * property files. If the file was not added, then the
     * FileNotFoundException will be thrown.
     */

public void removePropertiesFile (String fileName) throws FileNotFoundException;

    /**
     * Read in the properties file, if present.
     *
     * @param force if this is <code>true</code> the property files will
     * be resourced, i.e., properties will be re-loaded.
     */

public void loadProperties () throws SecurityException, IOException;

    /**
     * Set the property name to have the specified value.
     * If flush is true then we put the property value into the System
     * so that a call to System.getProperty will succeed. Otherwise we
     * cache it as a "volatile" value.
     * If <code>warn</code> is <code>true</code> then issue a warning
     * message if an existing property value is changed.
     */
    
public void setProperty (String name, String property, boolean flush, boolean warn);

    /**
     * Return the value associated with the property name. If the name
     * does not exist then the specified default value will be returned.
     */

public String getProperty (String name, String defaultValue);

    /**
     * Remove the property.
     */

public String removeProperty (String name);
	
}
