/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ORBObject.java,v 1.1.2.1.4.2 2001/01/11 14:16:48 nmcl Exp $
 */

package com.arjuna.OrbCommon;

import java.util.Properties;
import java.applet.Applet;
import java.io.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.ORBPackage.InvalidName;

/**
 * An attempt at some ORB portable ways of binding to objects and registering
 * objects with the name service, configuration file, etc.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBObject.java,v 1.1.2.1.4.2 2001/01/11 14:16:48 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class ORBObject
{

    /**
     * Bind to the object using the default (build-time specified) mechanism.
     *
     * @return the object reference.
     */

public static org.omg.CORBA.Object bindToObject (String objectName) throws org.omg.CORBA.ORBPackage.InvalidName, IOException, SystemException
    {
	return ORBServices.getService(objectName, null);
    }

    /**
     * Bind to the object using the specified location mechanism.
     * The location mechanisms are specified in ORBServices.
     *
     * @return the object reference.
     */
    
public static org.omg.CORBA.Object bindToObject (String objectName,
						     int locationMechanism) throws org.omg.CORBA.ORBPackage.InvalidName, IOException, SystemException
    {
	return ORBServices.getService(objectName, null, locationMechanism);
    }    

    /**
     * @return an IOR from the specified file, and create the object reference
     * from it.
     */
    
public static org.omg.CORBA.Object fileToObject (String objectName) throws org.omg.CORBA.ORBPackage.InvalidName, IOException, SystemException
    {
	return ORBServices.getService(objectName, null, ORBServices.FILE);
    }

    /**
     * From the given object reference, obtain an IOR and write it to the
     * specified file.
     */
    
public static void objectToFile (org.omg.CORBA.Object objRef,
				 String objectName) throws org.omg.CORBA.ORBPackage.InvalidName, IOException, SystemException
    {
	ORBServices.registerService(objRef, objectName, null, ORBServices.FILE);
    }

};
