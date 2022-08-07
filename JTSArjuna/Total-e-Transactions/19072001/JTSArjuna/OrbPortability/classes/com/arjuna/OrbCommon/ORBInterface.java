/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ORBInterface.javatmpl,v 1.3.4.1.2.2.4.1.2.1.2.1.20.3.2.2.2.1.10.1.30.1 2001/07/23 11:11:21 nmcl Exp $
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

import java.io.FileNotFoundException;
import java.io.IOException;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

/*
 * These can be removed once we remove the deprecated methods.
 */


import org.omg.PortableServer.*;
import org.omg.CORBA.Policy;


import org.omg.CORBA.ORBPackage.InvalidName;

import org.omg.PortableServer.POAPackage.AdapterAlreadyExists;
import org.omg.PortableServer.POAPackage.InvalidPolicy;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;


/**
 * An attempt at some ORB portable ways of interacting with the ORB.
 *
 * NOTE: initORB *must* be called if you want to use the
 * pre- and post- initialisation mechanisms.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBInterface.javatmpl,v 1.3.4.1.2.2.4.1.2.1.2.1.20.3.2.2.2.1.10.1.30.1 2001/07/23 11:11:21 nmcl Exp $
 * @since JTS 1.0.
 */

public class ORBInterface
{

    /**
     * Ensure that all ORB specific initialisation is done even if the
     * programmer uses the ORB specific init routines. This method does
     * not need to be called if using initORB.
     *
     * @since JTS 2.1.1.
     */

public static synchronized void init () throws SystemException
    {
	// null op - just to ensure we create the ORB object!
    }
    
    /**
     * Initialise the default ORB.
     */
    
public static synchronized void initORB () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::initORB ()");
	}

	/*
	 * Since an ORB can be initialised multiple times we currently
	 * allow the initialisation code to be activated multiple times
	 * as well. Does this make sense?
	 */
	
	if (!ORBInterface._orb.initialised())
	{
	    loadProperties();

	    ORBPreInitLoader preInit = new ORBPreInitLoader();
	    preInit = null;

	    parseORBProperties(null, false);

	    ORBInterface._orb.init();

	    parseORBProperties(null, true);
	    
	    ORBPostInitLoader postInit = new ORBPostInitLoader();
	    postInit = null;
	}
    }

    /**
     * Initialise the ORB.
     */

public static synchronized void initORB (Applet a, Properties p) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::initORB (Applet, Properties)");
	}

	if (!ORBInterface._orb.initialised())
	{
	    loadProperties();
	    
	    ORBPreInitLoader preInit = new ORBPreInitLoader();
	    preInit = null;

	    parseORBProperties(null, false);

	    ORBInterface._orb.init(a, p);

	    parseORBProperties(null, true);

	    ORBPostInitLoader postInit = new ORBPostInitLoader();
	    postInit = null;
	}
    }

    /**
     * Initialise the ORB.
     */

public static synchronized void initORB (String[] s, Properties p) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::initORB (String[], Properties)");
	}

	if (!ORBInterface._orb.initialised())
	{
	    loadProperties();
	    
	    ORBPreInitLoader preInit = new ORBPreInitLoader();
	    preInit = null;

	    parseORBProperties(s, false);

	    ORBInterface._orb.init(s, p);

	    parseORBProperties(s, true);
	    
	    ORBPostInitLoader postInit = new ORBPostInitLoader();
	    postInit = null;
	}
    }

public static synchronized boolean addORBAttribute (ORBAttribute p)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::addORBAttribute ("+p+")");
	}

	if (_orb.initialised())  // orb already set up!
	    return false;
	
	if (p.postORBInit())
	    _postORBInitProperty.put(p, p);
	else
	    _preORBInitProperty.put(p, p);

	return true;
    }

    /**
     * Shutdown the ORB.
     */
	
public static synchronized void shutdownORB ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::shutdownORB ()");
	}

	/*
	 * Do the cleanups first!
	 */

	if (!_preORBShutdown.isEmpty())
	{
	    Enumeration elements = _preORBShutdown.elements();

	    while (elements.hasMoreElements())
	    {
		ORBPreShutdown c = (ORBPreShutdown) elements.nextElement();

		if (c != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface - pre-orb shutdown on "+c.name());
		    }

		    c.work();
		    c = null;
		}
	    }

	    _preORBShutdown.clear();
	}

	if (_orb.initialised())
	    _orb.shutdown();

	if (!_postORBShutdown.isEmpty())
	{
	    Enumeration elements = _postORBShutdown.elements();

	    while (elements.hasMoreElements())
	    {
		ORBPostShutdown c = (ORBPostShutdown) elements.nextElement();

		if (c != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface - post-orb shutdown on "+c.name());
		    }

		    c.work();
		    c = null;
		}
	    }

	    _postORBShutdown.clear();
	}
    }
    
    /**
     * Obtain a reference to the current ORB.
     */
 
public static synchronized org.omg.CORBA.ORB orb ()
    {
	return _orb.orb();
    }

public static synchronized boolean setOrb (org.omg.CORBA.ORB theORB)
    {
	if (!_orb.initialised())
	{
	    _orb.orb(theORB);
	    
	    return true;
	}
	else
	    return false;
    }

public static synchronized void addPreORBShutdown (ORBPreShutdown c)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::addPreORBShutdown ("+c+")");
	}

	_preORBShutdown.put(c, c);
    }

public static synchronized void addPostORBShutdown (ORBPostShutdown c)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::addPostORBShutdown ("+c+")");
	}

	_postORBShutdown.put(c, c);
    }

    /*
     * Add any per-object filters which may have been registered.
     */

static synchronized void addFilters (org.omg.CORBA.Object obj)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::addFilters (Object obj)");
	}


    }

public static synchronized void destroyORB () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::destroyORB ()");
	}

	_orb.destroy();
    }
    
    /**
     * Place the server into a state where it can begin to
     * accept requests for objects from clients.
     */

public static void run (String name) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::run ("+name+")");
	}
	
	OAInterface.run(name);
    }
    
public static void run () throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::run ()");
	}

	OAInterface.run();
    }

    /**
     * Deprecated methods now available in the OAInterface class.
     */



    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void initPOA () throws SystemException
    {
	try
	{
	    OAInterface.initPOA();
	}
	catch (InvalidName e)
	{
	    throw new BAD_OPERATION();
	}
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */
    
public static synchronized void initPOA (String[] args) throws SystemException
    {
	try
	{
	    OAInterface.initPOA(args);
	}
	catch (InvalidName e)
	{
	    throw new BAD_OPERATION();
	}
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void createPOA (String adapterName,
					   Policy[] policies) throws AdapterAlreadyExists, InvalidPolicy, AdapterInactive
    {
	OAInterface.createPOA(adapterName, policies);
    }


    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static void initOA () throws SystemException
    {
	try
	{
	    OAInterface.initOA();
	}
	catch (InvalidName e)
	{
	    throw new BAD_OPERATION();
	}
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static void initOA (String[] args) throws SystemException
    {
	try
	{
	    OAInterface.initOA(args);
	}
	catch (InvalidName e)
	{
	    throw new BAD_OPERATION();
	}
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized boolean addOAAttribute (OAAttribute p)
    {
	return OAInterface.addOAAttribute(p);
    }



    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void destroyRootPOA () throws SystemException
    {
	OAInterface.destroyRootPOA();
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void destroyPOA () throws SystemException
    {
	OAInterface.destroyPOA("ChildPoa");
    }



    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static void shutdownOA () throws SystemException
    {
	OAInterface.shutdownOA();
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static void destroyOA () throws SystemException
    {
	OAInterface.destroyOA();
    }

    






    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized org.omg.PortableServer.POA rootPoa ()
    {
	return OAInterface.rootPoa();
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized org.omg.PortableServer.POA poa ()
    {
	return OAInterface.poa("ChildPoa");
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */
    
public static synchronized boolean setRootPoa (org.omg.PortableServer.POA thePOA)
    {
	return OAInterface.setRootPoa(thePOA);
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized boolean setPoa (org.omg.PortableServer.POA thePOA)
    {
	return OAInterface.setPoa("ChildPoa", thePOA);
    }





    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void addPreOAShutdown (OAPreShutdown c)
    {
	OAInterface.addPreOAShutdown(c);
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static synchronized void addPostOAShutdown (OAPostShutdown c)
    {
	OAInterface.addPostOAShutdown(c);
    }
 
    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Obtain a CORBA object reference for the Servant/Implementation.
     */


public static org.omg.CORBA.Object corbaReference (org.omg.PortableServer.Servant obj)
    {
	return OAInterface.corbaReference(obj);
    }
    
public static org.omg.CORBA.Object corbaReference (org.omg.PortableServer.Servant obj,
						       org.omg.PortableServer.POA poa)
    {
	return OAInterface.corbaReference(obj, poa);
    }

 
    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */


public static boolean objectIsReady (org.omg.PortableServer.Servant obj) throws SystemException
    {
	return OAInterface.objectIsReady(obj);
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */

public static boolean objectIsReady (org.omg.PortableServer.Servant obj, byte[] id) throws SystemException
    {
	return OAInterface.objectIsReady(obj, id);
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */

public static boolean objectIsReady (org.omg.PortableServer.Servant obj, byte[] id,
				     boolean rootPoa) throws SystemException
    {
	return OAInterface.objectIsReady(obj, id, (rootPoa ? null : "ChildPoa"));
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */

public static boolean objectIsReady (org.omg.PortableServer.Servant obj,
				     java.lang.Object[] params) throws SystemException
    {
	return OAInterface.objectIsReady(obj, params);
    }


    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */


public static boolean objectIsReady (org.omg.PortableServer.Servant obj, boolean rootPoa) throws SystemException

    {

	return OAInterface.objectIsReady(obj, (rootPoa ? null : "ChildPoa"));

    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Register the object with the ORB.
     */


public static boolean objectIsReady (org.omg.PortableServer.Servant obj,
				     java.lang.Object[] params,
				     boolean rootPoa) throws SystemException

    {

	return OAInterface.objectIsReady(obj, params, (rootPoa ? null : "ChildPoa"));

    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     *
     * Dispose of the object, i.e., unregister it from the ORB.
     */


public static boolean shutdownObject (org.omg.CORBA.Object obj)
    {
	return OAInterface.shutdownObject(obj);
    }


    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */


public static boolean shutdownObject (org.omg.CORBA.Object obj,
				      boolean rootPoa)

    {

	return OAInterface.shutdownObject(obj, (rootPoa ? null : "ChildPoa"));

    }



    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */

public static boolean shutdownObject (org.omg.PortableServer.Servant obj)
    {
	return OAInterface.shutdownObject(obj, null);
    }

    /**
     * @deprecated As of 2.1 this has moved to the OAInterface class.
     */
    
public static boolean shutdownObject (org.omg.PortableServer.Servant obj, boolean rootPoa)
    {
	return OAInterface.shutdownObject(obj, (rootPoa ? null : "ChildPoa"));
    }

    
protected ORBInterface ()
    {
    }

private static void loadProperties ()
    {
	try
	{
	    com.arjuna.ArjunaCommon.Common.PropertyManager.loadProperties();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("ORBInterface.loadProperties failed: "+e);


	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}
    }
    
private static void parseORBProperties (String[] params, boolean postInit)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ORB_PORTABILITY, "ORBInterface::parseORBProperties (String[], "+postInit+")");
	}

	Hashtable work = ((postInit) ? _postORBInitProperty : _preORBInitProperty);

	if (!work.isEmpty())
	{
	    Enumeration elements = work.elements();

	    while (elements.hasMoreElements())
	    {
		ORBAttribute p = (ORBAttribute) elements.nextElement();

		if (p != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ORB_PORTABILITY, "ORBAttribute "+p+" initialising.");
		    }

		    p.initialise(params);
		    p = null;
		}
	    }

	    work.clear();
	}
    }
    
static com.arjuna.orb.ORB _orb = new com.arjuna.orb.ORB();

private static Hashtable        _preORBShutdown = new Hashtable();
private static Hashtable        _postORBShutdown = new Hashtable();
private static Hashtable        _preORBInitProperty = new Hashtable();
private static Hashtable        _postORBInitProperty = new Hashtable();
private static ORBFilterManager _filterManager = new ORBFilterManager();

    static
    {
	addORBAttribute(new LocalSetup());
    }

}
