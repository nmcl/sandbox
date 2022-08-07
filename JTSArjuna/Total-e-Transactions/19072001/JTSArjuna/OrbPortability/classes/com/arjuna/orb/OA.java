/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OA.javatmpl,v 1.1.2.4.2.3.2.2.2.1.4.1.16.1 2001/06/15 12:15:49 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.orb;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.PropertyManager;

import org.omg.CORBA.Policy;

import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;


/**
 * An instance of this class provides access to the ORB specific
 * Object Adapter class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OA.javatmpl,v 1.1.2.4.2.3.2.2.2.1.4.1.16.1 2001/06/15 12:15:49 nmcl Exp $
 * @since JTS 2.1.
 */

public class OA
{

public OA (com.arjuna.orb.ORB theORB)
    {
	_theORB = theORB;
    }

public boolean supportsBOA ()
    {
	return _theOA.supportsBOA();
    }
    
public boolean supportsPOA ()
    {
	return _theOA.supportsPOA();
    }

public boolean initialised ()
    {
	return _theOA.initialised();
    }



public void init () throws InvalidName, AdapterInactive, SystemException
    {
	((POAImple) _theOA).init(_theORB);
    }

public void createPOA (String adapterName,
		       Policy[] policies) throws AdapterAlreadyExists, InvalidPolicy, AdapterInactive, SystemException
    {
	((POAImple) _theOA).createPOA(adapterName, policies);
    }

public void destroyRootPOA () throws SystemException
    {
	((POAImple) _theOA).destroyRootPOA();
    }
    
public void destroyPOA (String adapterName) throws SystemException
    {
	((POAImple) _theOA).destroyPOA(adapterName);
    }

public org.omg.PortableServer.POA rootPoa () throws SystemException
    {
	return ((POAImple) _theOA).rootPoa();
    }
    
public void rootPoa (org.omg.PortableServer.POA thePOA) throws SystemException
    {
	((POAImple) _theOA).rootPoa(thePOA);
    }
 
public org.omg.PortableServer.POA poa (String adapterName) throws SystemException
    {
	return ((POAImple) _theOA).poa(adapterName);
    }
    
public void poa (String adapterName, org.omg.PortableServer.POA thePOA) throws SystemException
    {
	((POAImple) _theOA).poa(adapterName, thePOA);
    }



public void run (String name) throws SystemException
    {
	_theOA.run(_theORB, name);
    }
    
public void run () throws SystemException
    {
	_theOA.run(_theORB);
    }

private com.arjuna.orb.ORB _theORB;
    
private static com.arjuna.orb.OAImple _theOA;
    
    static
    {
	String className = PropertyManager.getProperty(com.arjuna.OrbCommon.ORBEnvironment.OA_IMPLEMENTATION);
	
	if (className == null)
	{






	    className = "com.arjuna.orb.implementations.OA.orbix2000_";







	    className = className+ "1_2" ;
	}

	try
	{
	    Class c = Class.forName(className);
	
	    _theOA = (OAImple) c.newInstance();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("OA ORB specific class creation failed with: "+e);

	    throw new ExceptionInInitializerError("OA ORB specific class creation failed with: "+e);
	}
    }
 
}

