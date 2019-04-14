/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Configuration.javatmpl,v 1.1 2003/06/19 10:50:14 nmcl Exp $
 */

package com.arjuna.ats.arjuna.common;

import com.hp.mw.common.util.propertyservice.PropertyManager;
import java.io.File;

import com.arjuna.ats.arjuna.logging.tsLogger;

import java.io.IOException;

/*
 * When we have an installation utility (e.g., InstallShield) we can have
 * that compile the property file location in a separate file which this
 * class then uses. So we ship a small source file in the distribution which
 * then gets built on a per-installation basis. Could do that for other
 * things too.
 */

/**
 * This class contains various run-time configuration options. Default
 * values are provided at compile-time, and may be operating system
 * specific.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Configuration.javatmpl,v 1.1 2003/06/19 10:50:14 nmcl Exp $
 * @since 1.0.
 */

public class Configuration
{

    /**
     * @return the name of the module properties file to use.
     */

public static synchronized final String propertiesFile ()
    {
	return _propFile;
    }

    /**
     * Set the name of the properties file.
     */

public static synchronized final void setPropertiesFile (String file)
    {
	_propFile = file;
    }

    /**
     * @return the location of the module properties file to use.
     *
     * @message com.arjuna.ats.arjuna.common.Configuration_1 [com.arjuna.ats.arjuna.common.Configuration_1] Configuration.propertiesDir() - Cannot find properties file {0}
     */

public static synchronized final String propertiesDir ()
    {
        String propDir = ".";

        try
        {
            propDir = arjPropertyManager.propertyManager.locateFile( propertiesFile() );
        }
        catch (java.io.FileNotFoundException e)
        {
            if (tsLogger.arjLoggerI18N.isWarnEnabled()) {
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.common.Configuration_1", 
					   new Object[]{propertiesFile()});
            }
        }

	return propDir;
    }

    /**
     * @return the version of arjuna.
     */

public static final String version ()
    {
	return "unknown";
    }

    /**
     * Used to obtain the root of the object store.
     *
     * @return <code>path</code> to object store.
     */

public static synchronized final String objectStoreRoot ()
    {
	if (_objectStore == null)
	{
	    /* Set default location under current directory */
	    _objectStore = System.getProperty("user.dir") + File.separator + "ObjectStore";

	}

	return _objectStore;
    }

    /**
     * Used to set the root of the object store. Changes will
     * take effect the next time the root is queried. Existing
     * object store instances will not be effected.
     */

public static synchronized final void setObjectStoreRoot (String s)
    {
	_objectStore = s;
    }

    /**
     * Whether to use the alternative abstract record ordering.
     * At present this is not fully documented, so stay away!
     *
     * @return <code>true</code> if order abstract records on type first, or
     * <code>false</code> if order on Uid first.
     */

public static synchronized final boolean useAlternativeOrdering ()
    {
	return _useAltOrder;
    }

    /**
     * Set whether or not to use the alternative abstract record
     * ordering. Takes effect the next time ordering is required.
     */

public static synchronized final void setAlternativeOrdering (boolean b)
    {
	_useAltOrder = b;
    }

private static String  _objectStore = null;
private static boolean _useAltOrder = false;
private static String  _propFile = "arjuna-properties.xml";


}
