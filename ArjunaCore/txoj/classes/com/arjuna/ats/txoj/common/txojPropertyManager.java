/*
 * Copyright (C) 2002
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: txojPropertyManager.java,v 1.2 2003/08/11 14:08:17 nmcl Exp $
 */

package com.arjuna.ats.txoj.common;

import com.arjuna.common.util.propertyservice.PropertyManager;

/**
 * Property manager wrapper for the Arjuna module.
 *
 * @author Richard Begg (richard_begg@hp.com)
 */
public class txojPropertyManager
{
    public static PropertyManager  propertyManager;

    static
    {
        /**
         * Retrieve the property manager from the factory and add the TXOJ
	 * properties file to it.
         */

        propertyManager = PropertyManager.getPropertyManager("com.arjuna.ats.propertymanager");

        String propertiesFilename = System.getProperty(Environment.PROPERTIES_FILE);

        if (propertiesFilename == null)
            propertiesFilename = Configuration.propertiesFile();

        propertyManager.addPropertiesFile(propertiesFilename);

        try
        {
            propertyManager.loadProperties(true);
        }
        catch (Exception e)
        {
            throw new ExceptionInInitializerError(e.toString());
        }
    }
}
