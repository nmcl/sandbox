/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Configuration.javatmpl,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 */

package com.arjuna.ats.txoj.common;

import com.hp.mw.common.util.ClassPathParser;
import com.hp.mw.common.util.propertyservice.PropertyManager;
import java.io.File;

import java.io.IOException;

/**
 * Runtime configuration class for this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Configuration.javatmpl,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 * @since JTS 1.0.
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
     * Used to get the root of the object store.
     *
     * @return <code>path</code> to lock store.
     */

public static synchronized final String lockStoreRoot ()
    {
	if (_lockStore == null)
	    /* Set default location under current directory */
	    _lockStore = System.getProperty("user.dir") + File.separator + "LockStore";

System.err.println("Returning lockstore location of: " + _lockStore);
	return _lockStore;
    }

    /**
     * Used to set the root of the lock store. Changes will
     * take effect the next time the root is queried. Existing
     * lock store instances will not be effected.
     */

public static synchronized final void setLockStoreRoot (String s)
    {
	_lockStore = s;
    }

public static final String version ()
    {
	return "unknown";
    }

private static String _lockStore = null;
private static String _propFile = "txoj-properties.xml";
    
}

