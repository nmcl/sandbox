/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OAInterface.javatmpl,v 1.1.2.4.2.4.2.1.2.1.4.3.38.1 2001/08/09 10:34:43 nmcl Exp $
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
































































package com.arjuna.OrbCommon;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.orb.ORB;
import com.arjuna.orb.OA;
import java.util.*;
import java.applet.Applet;
import java.io.*;
import org.omg.PortableServer.*;
import org.omg.CORBA.Policy;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;

/**
 * An attempt at some ORB portable ways of interacting with the OA.
 *
 * NOTE: initPOA *must* be called if you want to use the
 * pre- and post- initialisation mechanisms.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAInterface.javatmpl,v 1.1.2.4.2.4.2.1.2.1.4.3.38.1 2001/08/09 10:34:43 nmcl Exp $
 * @since JTS 2.1.
 */

public class OAInterface
{

    /**
     * Ensure that all OA specific initialisation is done even if the
     * programmer uses the OA specific init routines. This method does
     * not need to be called if using initOA methods.
     *
     * @since JTS 2.1.1.
     */

public static synchronized void init () throws SystemException
    {
	// null op - just to ensure we create the OA object!
    }



public static synchronized void initPOA () throws InvalidName, SystemException
    {
	initPOA(null);
    }
    
public static synchronized void initPOA (String[] args) throws InvalidName, SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::initPOA (String[])");
	}

	if (!OAInterface._oa.initialised())
	{
	    if (ORBInterface._orb.initialised())
	    {
		parseOAProperties(args, true);

		try
		{
		    _oa.init();  // create the root poa
		}
		catch (Exception e)
		{
		    ErrorStream.fatal().println("OAInterface.initPOA caught: "+e);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
		}
		
		parseOAProperties(args, false);
	    }
	    else
	    {
		ErrorStream.fatal().println("OAInterface.initPOA called without initialised ORB.");

		throw new com.arjuna.ArjunaCommon.Common.FatalError("initORB not called!");
	    }
	}
    }

public static synchronized void createPOA (String adapterName,
					   Policy[] policies) throws AdapterAlreadyExists, InvalidPolicy, AdapterInactive
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::createPOA ("+adapterName+" )");
	}

	if (!_oa.initialised())
	{
	    ErrorStream.warning().println("OAInterface.createPOA called without root POA.");

	    throw new AdapterInactive();
	}

	if (_defaultAdapterName == null)
	    _defaultAdapterName = adapterName;
	
	_oa.createPOA(adapterName, policies);
    }



public static void initOA () throws InvalidName, SystemException
    {
	initOA(null);
    }

public static void initOA (String[] args) throws InvalidName, SystemException
    {

	initPOA(args);

    }

public static synchronized boolean addOAAttribute (OAAttribute p)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::addOAAttribute ("+p+")");
	}

	if ((_oa.initialised()) || (p == null))  // oa already set up!
	    return false;
	
	if (p.postOAInit())
	    _postOAInitProperty.put(p, p);
	else
	    _preOAInitProperty.put(p, p);

	return true;
    }



public static synchronized void destroyRootPOA () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::destroyRootPOA ()");
	}

	if (!_oa.initialised())
	{
	    if (!_preOAShutdown.isEmpty())
	    {
		Enumeration elements = _preOAShutdown.elements();

		while (elements.hasMoreElements())
		{
		    OAPreShutdown c = (OAPreShutdown) elements.nextElement();

		    if (c != null)
		    {
			c.work();
			c = null;
		    }
		}

		_preOAShutdown.clear();
	    }

	    _oa.destroyRootPOA();

	    if (!_postOAShutdown.isEmpty())
	    {
		Enumeration elements = _postOAShutdown.elements();
		
		while (elements.hasMoreElements())
		{
		    OAPostShutdown c = (OAPostShutdown) elements.nextElement();

		    if (c != null)
		    {
			c.work();
			c = null;
		    }
		}
		
		_postOAShutdown.clear();
	    }
	}
    }

public static synchronized void destroyPOA (String adapterName) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::destroyPOA ()");
	}

	if (adapterName == null)
	    throw new BAD_PARAM();
	
	_oa.destroyPOA(adapterName);
    }



public static void shutdownOA () throws SystemException
    {

	// nothing equivalent for POA

	if (!_preOAShutdown.isEmpty())
	{
	    Enumeration elements = _preOAShutdown.elements();

	    while (elements.hasMoreElements())
	    {
		OAPreShutdown c = (OAPreShutdown) elements.nextElement();

		if (c != null)
		{
		    c.work();
		    c = null;
		}
	    }

	    _preOAShutdown.clear();
	}

	if (!_postOAShutdown.isEmpty())
	{
	    Enumeration elements = _postOAShutdown.elements();

	    while (elements.hasMoreElements())
	    {
		OAPostShutdown c = (OAPostShutdown) elements.nextElement();

		if (c != null)
		{
		    c.work();
		    c = null;
		}
	    }

	    _postOAShutdown.clear();
	}

    }

public static void destroyOA () throws SystemException
    {

	destroyRootPOA();

    }
    

    




public static synchronized org.omg.PortableServer.POA rootPoa ()
    {
	return _oa.rootPoa();
    }

public static synchronized org.omg.PortableServer.POA poa (String adapterName)
    {
	return _oa.poa(adapterName);
    }
    
public static synchronized boolean setRootPoa (org.omg.PortableServer.POA thePOA)
    {
	if (!_oa.initialised())
	{
	    _oa.rootPoa(thePOA);

	    return true;
	}
	else
	    return false;
    }

public static synchronized boolean setPoa (String adapterName, org.omg.PortableServer.POA thePOA)
    {
	if (adapterName != null)
	{
	    _oa.poa(adapterName, thePOA);

	    return true;
	}
	else
	    return false;
    }





public static synchronized void addPreOAShutdown (OAPreShutdown c)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::addPreOAShutdown ("+c+")");
	}

	_preOAShutdown.put(c, c);
    }

public static synchronized void addPostOAShutdown (OAPostShutdown c)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::addPostOAShutdown ("+c+")");
	}

	_postOAShutdown.put(c, c);
    }
 
    /**
     * @return a CORBA object reference for the Servant/Implementation.
     */


public static org.omg.CORBA.Object corbaReference (org.omg.PortableServer.Servant obj)
    {
	return corbaReference(obj, (String) null);
    }
    
public static org.omg.CORBA.Object corbaReference (org.omg.PortableServer.Servant obj, String adapterName)
    {
	org.omg.CORBA.Object objRef = null;
	
	if ((adapterName == null) || (adapterName.equals("RootPOA")))
	{
	    try
	    {
		objRef = corbaReference(obj, _oa.rootPoa());
	    }
	    catch (Exception e)
	    {
		objRef = null;
	    }
	}
	else
	{
	    if (_oa.initialised())
	    {
		try
		{
		    objRef = corbaReference(obj, _oa.poa(adapterName));
		}
		catch (Exception e)
		{
		    objRef = null;
		}
	    }
	}
	
	return objRef;
    }
    
public static org.omg.CORBA.Object corbaReference (org.omg.PortableServer.Servant obj,
						   org.omg.PortableServer.POA poa)
    {
	try
	{
	    return poa.servant_to_reference(obj);
	}
	catch (Exception e)
	{
	    return null;
	}
    }

 
    /**
     * Register the object with the ORB.
     */


public static boolean objectIsReady (org.omg.PortableServer.Servant obj) throws SystemException
    {
	return objectIsReady(obj, (String) null);
    }

public static boolean objectIsReady (org.omg.PortableServer.Servant obj, byte[] id) throws SystemException
    {
	return objectIsReady(obj, id, (String) null);
    }

public static boolean objectIsReady (org.omg.PortableServer.Servant obj, byte[] id,
				     String adapterName) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::objectIsReady (Servant, byte[], "+adapterName+")");
	}
	
	try
	{
	    if ((adapterName == null) || (adapterName.equals("RootPOA")))
	    {
		_oa.rootPoa().activate_object_with_id(id, obj);
	    }
	    else
		_oa.poa(adapterName).activate_object_with_id(id, obj);
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OAInterface.objectIsReady: caught "+e+ " for " + obj);

	    return false;
	}

	return true;
    }

public static boolean objectIsReady (org.omg.PortableServer.Servant obj,
				     java.lang.Object[] params) throws SystemException
    {
	return objectIsReady(obj, params, (String) null);
    }



public static boolean objectIsReady (org.omg.PortableServer.Servant obj, String adapterName) throws SystemException

    {
	if (DebugController.enabled())
	{

	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::objectIsReady (Servant, "+adapterName+")");

	}

	boolean result = true;
	
	try
	{

	    boolean invalidPOA = false;
	    boolean rootPoa = ((adapterName == null) || (adapterName.equals("RootPOA")));
	    
	    if (rootPoa)
	    {
		if (_oa.initialised())
		{
		    _oa.rootPoa().activate_object(obj);
		}
		else
		    invalidPOA = true;
	    }
	    else
	    {
		if (_oa.poa(adapterName) != null)
		    _oa.poa(adapterName).activate_object(obj);
		else
		    invalidPOA = true;
	    }
	    
	    if (invalidPOA)
	    {
		ErrorStream.warning().println("OAInterface.objectIsReady - invalid POA: "+((rootPoa) ? "rootPOA" : "childPOA"));

		result = false;
	    }


	    com.arjuna.OrbCommon.event.EventManager.getManager().connected(corbaReference(obj));
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OAInterface.objectIsReady: caught "+e + " for, no id and " + obj);

	    result = false;
	}
	
	return result;
    }


public static boolean objectIsReady (org.omg.PortableServer.Servant obj,
				     java.lang.Object[] params,
				     String adapterName) throws SystemException

    {
	if (DebugController.enabled())
	{

	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::objectIsReady (Servant, java.lang.Object[], "+adapterName+")");

	}


	return objectIsReady(obj, adapterName);

    }

    /**
     * Dispose of the object, i.e., unregister it from the ORB.
     */


public static boolean shutdownObject (org.omg.CORBA.Object obj)
    {
	return shutdownObject(obj, (String) null);
    }



public static boolean shutdownObject (org.omg.CORBA.Object obj,
				      String adapterName)

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::shutdownObject ()");
	}
	
	boolean result = true;
	
	try
	{
	    com.arjuna.OrbCommon.event.EventManager.getManager().disconnected(obj);


	    boolean invalidPOA = false;
	    boolean rootPoa = ((adapterName == null) || (adapterName.equals("RootPOA")));
	    
	    if (rootPoa)
	    {
		if (_oa.initialised())
		    _oa.rootPoa().deactivate_object(_oa.rootPoa().reference_to_id(obj));
		else
		    invalidPOA = true;
	    }
	    else
	    {
		if (_oa.poa(adapterName) != null)
		    _oa.poa(adapterName).deactivate_object(_oa.poa(adapterName).reference_to_id(obj));
		else
		    invalidPOA = true;
	    }

	    if (invalidPOA)
	    {
		ErrorStream.warning().println("OAInterface.shutdownObject - invalid POA: "+((rootPoa) ? "rootPOA" : "childPOA"));

		result = false;
	    }

	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OAInterface.shutdownObject: caught "+e);

	    result = false;
	}

	return result;
    }


public static boolean shutdownObject (org.omg.PortableServer.Servant obj)
    {
	return shutdownObject(obj, null);
    }
    
public static boolean shutdownObject (org.omg.PortableServer.Servant obj, String adapterName)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::shutdownObject ("+adapterName+")");
	}

	boolean result = true;
	
	try
	{
	    boolean invalidPOA = false;
	    boolean rootPoa = ((adapterName == null) || (adapterName.equals("RootPOA")));
	    
	    if (rootPoa)
	    {
		if (_oa.initialised())
		    _oa.rootPoa().deactivate_object(_oa.rootPoa().servant_to_id(obj));
		else
		    invalidPOA = true;
	    }
	    else
	    {
		if (_oa.poa(adapterName) != null)
		    _oa.poa(adapterName).deactivate_object(_oa.poa(adapterName).servant_to_id(obj));
		else
		    invalidPOA = true;
	    }

	    if (invalidPOA)
	    {
		ErrorStream.warning().println("OAInterface.shutdownObject - invalid POA: "+((rootPoa) ? "rootPOA" : "childPOA"));

		result = false;
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println("OAInterface.shutdownObject: caught "+e);

	    result = false;
	}

	return result;
    }

    
    /**
     * Place the server into a state where it can begin to
     * accept requests for objects from clients.
     */

static void run (String name) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::run ("+name+")");
	}

	_oa.run(name);
    }
    
static void run () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::run ()");
	}

	_oa.run();
    }
	     
protected OAInterface ()
    {
    }
    
private static void parseOAProperties (String[] params, boolean preInit)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "OAInterface::parseOAProperties (String[], "+preInit+")");
	}

	Hashtable work = ((preInit) ? _preOAInitProperty : _postOAInitProperty);

	if (!work.isEmpty())
	{
	    Enumeration elements = work.elements();

	    while (elements.hasMoreElements())
	    {
		OAAttribute p = (OAAttribute) elements.nextElement();

		if (p != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ORB_PORTABILITY, "OAAttribute "+p+" initialising.");
		    }

		    p.initialise(params);
		    p = null;
		}
	    }

	    work.clear();
	}
    }    
    
static com.arjuna.orb.OA _oa = new com.arjuna.orb.OA(ORBInterface._orb);


private static String    _defaultAdapterName = null;


private static Hashtable _preOAShutdown = new Hashtable();
private static Hashtable _postOAShutdown = new Hashtable();    
private static Hashtable _preOAInitProperty = new Hashtable();
private static Hashtable _postOAInitProperty = new Hashtable();    

}
