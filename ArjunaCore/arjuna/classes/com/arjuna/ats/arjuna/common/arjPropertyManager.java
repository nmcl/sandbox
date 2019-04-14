/*
 * Copyright (C) 2002
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: arjPropertyManager.java,v 1.2 2003/08/11 14:07:36 nmcl Exp $
 */

package com.arjuna.ats.arjuna.common;

import com.arjuna.common.util.propertyservice.PropertyManager;

/**
 * Property manager wrapper for the Arjuna module.
 *
 * @author Richard Begg (richard_begg@hp.com)
 */
public class arjPropertyManager
{
    public static PropertyManager  propertyManager;

    static
    {
        /**
         * Retrieve the property manager from the factory and add the Arjuna properties file to it.
         */
        propertyManager = propertyManager.getPropertyManager("com.arjuna.ats.propertymanager");

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
