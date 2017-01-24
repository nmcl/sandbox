/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBObject.javatmpl,v 1.4 1998/11/12 08:52:36 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_krc.h,v 1.3 1999/09/10 13:49:11 nmcl Exp $
 */













































package com.arjuna.OrbCommon;

import java.util.Properties;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.*;


import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.SystemException;
import org.omg.CosNaming.NamingContextPackage.InvalidName;
import org.omg.CosNaming.NamingContextPackage.CannotProceed;
import org.omg.CosNaming.NamingContextPackage.NotFound;

/**
 * An attempt at some ORB portable ways of binding to objects and registering
 * objects with the name service, configuration file, etc.
 */

public class ORBObject
{

    /**
     * Bind to the object using the default (build-time specified) mechanism.
     */
    
public static org.omg.CORBA.Object bindToObject (String objectName) throws InvalidName, CannotProceed, NotFound, IOException
    {
	return ORBServices.getService(objectName, null);
    }

    /**
     * Bind to the object using the specified location mechanism.
     * The location mechanisms are specified in ORBServices.
     */
    
public static org.omg.CORBA.Object bindToObject (String objectName, int locationMechanism) throws InvalidName, CannotProceed, NotFound, IOException
    {
	return ORBServices.getService(objectName, null, locationMechanism);
    }    

    /**
     * Obtain an IOR from the specified file, and create the object reference
     * from it.
     */
    
public static org.omg.CORBA.Object fileToObject (String objectName) throws InvalidName, CannotProceed, NotFound, IOException
    {
	return ORBServices.getService(objectName, null, ORBServices.FILE);
    }

    /**
     * From the given object reference, obtain an IOR and write it to the
     * specified file.
     */
    
public static void objectToFile (org.omg.CORBA.Object objRef, String objectName) throws NotFound, CannotProceed, InvalidName, IOException
    {
	ORBServices.registerService(objRef, objectName, null, ORBServices.FILE);
    }

};
