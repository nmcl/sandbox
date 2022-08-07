/*
 * Copyright (C) 2001
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryConfiguration.javatmpl,v 1.1.2.1.32.1 2001/06/20 10:32:20 nmcl Exp $
 */

package com.arjuna.CosRecovery;

/*
 *  see ArjunaCommon/Common/Configuration.javatmpl for anticipated use
 */
 
/**
 * This class contains various run-time configuration options. Default
 * values are provided at compile-time, and may be operating system
 * specific.
 *
 * @author Peter Furniss (peter.furniss@arjuna.com)
 * @version $Id: RecoveryConfiguration.javatmpl,v 1.1.2.1.32.1 2001/06/20 10:32:20 nmcl Exp $
 * @since JTS 1.0.
 */
public class RecoveryConfiguration
{

    /**
     * @return the name of the RecoveryManager properties file to use.
     * @since JTS 2.1.
     */
public static synchronized final String recoveryManagerPropertiesFile()
    {
	return _rmPropertyFile;
    }

    /**
     * Set the name of the RecoveryManager properties file.
     * @since JTS 2.1.
     */

public static synchronized final void setRecoveryManagerPropertiesFile (String file)
    {
	_rmPropertyFile = file;
    }


private    static String _rmPropertyFile = "RecoveryManager_2_2.properties"  ;
    
private RecoveryConfiguration()
{
    // zero-ton class 
}

};



