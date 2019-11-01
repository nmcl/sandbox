/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Configuration.javatmpl,v 1.1.4.2.6.1.4.3 2001/01/30 10:35:51 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import com.arjuna.ArjunaCommon.Common.ClassPathParser;
import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import java.io.File;

import java.io.IOException;

/**
 * Runtime configuration class for this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Configuration.javatmpl,v 1.1.4.2.6.1.4.3 2001/01/30 10:35:51 nmcl Exp $
 * @since JTS 1.0.
 */

public class Configuration
{

    /**
     * Used to obtain the root of the object store.
     *
     * @return <code>path</code> to object store.
     */

public static synchronized final String objectStoreRoot ()
    {
	if (_objectStore == null)
	{
	    ClassPathParser cp = new ClassPathParser();
	    
	    _objectStore = cp.getPath("/ObjectStore");

	    if (_objectStore == null)
	    {
		/*
		 * Find the location of the root property file.
		 */

		try
		{
		    PropertyManager.loadProperties();
		}
		catch (IOException e)
		{
		    ErrorStream.warning().println(e.toString());
		}
		
		String pathName = com.arjuna.ArjunaCommon.Common.Configuration.propertiesDir();
		
		if (pathName != null)
		{
		    /*
		     * Strip off the etc part.
		     */

		    int indx = pathName.lastIndexOf(File.separatorChar);

		    /*
		     * Just in case the user is mixing separators!
		     */

		    if (indx == -1)
		    {
			if (com.arjuna.ArjunaCommon.Common.Utility.isWindows())
			    indx = pathName.lastIndexOf('/');
		    }
		    
		    if (indx != -1)
		    {
			if (indx == 0) // properties are in /??!!
			    _objectStore = null;
			else
			    _objectStore = pathName.substring(0, indx);
		    
			_objectStore = _objectStore + File.separator + "ObjectStore";
		    }
		}
	    }
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
     * Used to get the root of the object store.
     *
     * @return <code>path</code> to lock store.
     */

public static synchronized final String lockStoreRoot ()
    {
	if (_lockStore == null)
	{
	    ClassPathParser cp = new ClassPathParser();
	    
	    _lockStore = cp.getPath("/LockStore");

	    if (_lockStore == null)
	    {
		String pathName = com.arjuna.ArjunaCommon.Common.Configuration.propertiesDir();
		
		if (pathName != null)
		{
		    /*
		     * Strip off the etc part.
		     */

		    int indx = pathName.lastIndexOf(File.separatorChar);

		    /*
		     * Just in case the user is mixing separators!
		     */

		    if (indx == -1)
		    {
			if (com.arjuna.ArjunaCommon.Common.Utility.isWindows())
			    indx = pathName.lastIndexOf('/');
		    }

		    if (indx != -1)
		    {
			if (indx == 0) // properties are in /??!!
			    _lockStore = null;
			else
			    _lockStore = pathName.substring(0, indx);
		
			_lockStore = _lockStore + File.separator + "LockStore";
		    }
		}
	    }
	    
	}

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

    /**
     * Returns the version of ArjunaCore.
     */
    
public static final String version ()
    {
	return "4.7.0" ;
    }

private static String  _objectStore = null;
private static String  _lockStore = null;
private static boolean _useAltOrder = false;
    
}

