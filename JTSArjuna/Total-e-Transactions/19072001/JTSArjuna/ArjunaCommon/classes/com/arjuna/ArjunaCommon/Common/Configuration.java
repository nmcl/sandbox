/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Configuration.javatmpl,v 1.1.28.2.4.2.4.2 2001/01/26 13:41:58 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

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
 * @version $Id: Configuration.javatmpl,v 1.1.28.2.4.2.4.2 2001/01/26 13:41:58 nmcl Exp $
 * @since JTS 1.0.
 */

public class Configuration
{

    /**
     * @return the name of the module properties file to use.
     * @since JTS 2.1 this is different - prior versions used to return
     * the entire path.
     */

public static synchronized final String propertiesFile ()
    {
	return _propFile;
    }

    /**
     * Set the name of the properties file.
     * @since JTS 2.1 this is different - prior versions used to set
     * the entire path.
     */

public static synchronized final void setPropertiesFile (String file)
    {
	_propFile = file;
    }

    /**
     * @return the location of the module properties file to use.
     * @since JTS 2.1.
     */

public static synchronized final String propertiesDir ()
    {
	return _propDir;
    }

    /**
     * Set the location of the properties file.
     * @since JTS 2.1.
     */

public static synchronized final void setPropertiesDir (String file)
    {
	_propDir = file;
    }

    /**
     * @return the version of ArjunaCommon.
     */

public static final String version ()
    {
	return "4.3.0" ;
    }

private static String _propFile = "TransactionService_2_2.properties" ;
private static String _propDir = "C:/PROGRA~1/Arjuna/TransactionService/etc" ;
	
}
