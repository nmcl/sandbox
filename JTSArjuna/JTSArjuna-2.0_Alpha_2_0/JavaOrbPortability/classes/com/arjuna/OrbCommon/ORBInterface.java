/*
 * Copyright (C) 1997, 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ORBInterface.javatmpl,v 1.11.2.2.4.2.2.4 2000/01/27 15:39:08 nmcl Exp $
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

import com.arjuna.JavaGandiva.Common.Utility;
import java.util.*;
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
 * An attempt at some ORB portable ways of interacting with the ORB
 * and BOA.
 *
 * NOTE: initORB and initBOA *must* be called if you want to use the
 * pre- and post- initialisation mechanisms.
 */

public class ORBInterface
{

    /**
     * Initialise the default ORB.
     */
    
public static synchronized void initORB () throws SystemException
    {
	/*
	 * Since an ORB can be initialised multiple times we currently
	 * allow the initialisation code to be activated multiple times
	 * as well. Does this make sense?
	 */
	
	if (ORBInterface._orb == null)
	{
	    com.arjuna.JavaGandiva.Common.Utility.loadProperties();

	    ORBPreInit preInit = new ORBPreInit();
	    preInit = null;

	    setORBInit(null, false);
	    ORBInterface._orb = org.omg.CORBA.ORB.init();

	    ((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);

	    setORBInit(null, true);
	    
	    ORBPostInit postInit = new ORBPostInit();
	    postInit = null;
	}
    }

    /**
     * Initialise the ORB.
     */
    
public static synchronized void initORB (Applet a, Properties p) throws SystemException
    {

	if (ORBInterface._orb == null)
	{
	    com.arjuna.JavaGandiva.Common.Utility.loadProperties();

	    ORBPreInit preInit = new ORBPreInit();
	    preInit = null;

	    setORBInit(null, false);
	    ORBInterface._orb = org.omg.CORBA.ORB.init(a, p);

	    ((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);

	    setORBInit(null, true);

	    ORBPostInit postInit = new ORBPostInit();
	    postInit = null;
	}

    }

    /**
     * Initialise the ORB.
     */
    
public static synchronized void initORB (String[] s, Properties p) throws SystemException
    {

	if (ORBInterface._orb == null)
	{
	    com.arjuna.JavaGandiva.Common.Utility.loadProperties();

	    ORBPreInit preInit = new ORBPreInit();
	    preInit = null;

	    setORBInit(s, false);
	    ORBInterface._orb = org.omg.CORBA.ORB.init(s, p);

	    ((com.ooc.CORBA.ORB) ORBInterface._orb).conc_model(com.ooc.CORBA.ORB.ConcModel.ConcModelThreaded);

	    setORBInit(s, true);
	    
	    ORBPostInit postInit = new ORBPostInit();
	    postInit = null;
	}

    }    

    /**
     * Initialise the BOA.
     */
      
public static synchronized void initBOA () throws SystemException
    {

	if (ORBInterface._boa == null)
	{
	    if (ORBInterface._orb != null)
	    {
		setBOAInit(null, true);

		String[] args = new String[0];
		

	        ORBInterface._boa = ((com.ooc.CORBA.ORB) _orb).BOA_init(args, new java.util.Properties());
		((com.ooc.CORBA.BOA) ORBInterface._boa).conc_model(com.ooc.CORBA.BOA.ConcModel.ConcModelThreadPerRequest);
		((com.ooc.CORBA.BOA) ORBInterface._boa).init_servers();


		setBOAInit(null, false);
	    }
	    else
	    {
		System.err.println("ERROR - initBOA called without initialised ORB.");
		System.exit(0);
	    }
	}

    }
    
    /**
     * Initialise the BOA.
     */
    
public static synchronized void initBOA (String a, Properties p) throws SystemException
    {

	if (ORBInterface._boa == null)
	{
	    if (ORBInterface._orb != null)
	    {

		if (ORBDebugController.debugAllowed())
		{
		    System.err.println("WARNING - initBOA(String) not supported by ORB. Using initBOA(String[])");
		}

		String[] args = new String[1];
		args[0] = a;

		initBOA(args, p);

		args = null;

	    }
	    else
	    {
		System.err.println("ERROR - initBOA called without initialised ORB.");
		System.exit(0);
	    }
	}

    }

    /**
     * Initialise the BOA.
     */
    
public static synchronized void initBOA (String[] a, Properties p) throws SystemException
    {

	if (ORBInterface._boa == null)
	{
	    if (ORBInterface._orb != null)
	    {

		setBOAInit(null, true);

		ORBInterface._boa = ((com.ooc.CORBA.ORB) _orb).BOA_init(a, p);

		((com.ooc.CORBA.BOA) ORBInterface._boa).conc_model(com.ooc.CORBA.BOA.ConcModel.ConcModelThreadPerRequest);
		((com.ooc.CORBA.BOA) ORBInterface._boa).init_servers();

		setBOAInit(null, false);

	    }
	    else
	    {
		System.err.println("ERROR - initBOA called without initialised ORB.");
		System.exit(0);
	    }
	}

    }    

public static synchronized boolean addORBInit (ORBInit p)
    {
	if (_orb != null)  // orb already set up!
	    return false;
	
	if (p.postORBInit())
	    postORBInitProperty.put(p, p);
	else
	    preORBInitProperty.put(p, p);

	return true;
    }

public static synchronized boolean addBOAInit (BOAInit p)
    {

	if (_boa != null)  // boa already set up!
	    return false;
	
	if (p.postBOAInit())
	    postBOAInitProperty.put(p, p);
	else
	    preBOAInitProperty.put(p, p);

	return true;

    }    

    /**
     * Shutdown the ORB.
     */
	
public static synchronized void shutdownORB ()
    {
	/*
	 * Do the cleanups first!
	 */

	if (!cleanups.isEmpty())
	{
	    Enumeration elements = cleanups.elements();

	    while (elements.hasMoreElements())
	    {
		ORBCleanup c = (ORBCleanup) elements.nextElement();

		if (c != null)
		{
		    c.cleanup();
		    c = null;
		}
	    }

	    cleanups.clear();
	}

	if (_orb != null)
	{



	    _orb = null;
	}
    }

    /**
     * Shutdown the BOA.
     */

public static synchronized void shutdownBOA (String name)
    {

	if (_boa != null)
	{


	    _boa.deactivate_impl(null);

	    _boa = null;
	}

    }

public static synchronized void shutdownBOA ()
    {
	shutdownBOA(null);
    }

    /**
     * Obtain a reference to the current ORB.
     */
 
public static synchronized org.omg.CORBA.ORB orb ()
    {
	return _orb;
    }

public static synchronized boolean setOrb (org.omg.CORBA.ORB theORB)
    {
	if (_orb == null)
	{
	    _orb = theORB;
	    
	    return true;
	}
	else
	    return false;
    }


    /**
     * Obtain a reference to the current BOA.
     */
  
public static synchronized org.omg.CORBA.BOA boa ()
    {
	return _boa;
    }

public static synchronized boolean setBoa (org.omg.CORBA.BOA theBOA)
    {
	if (_boa == null)
	{
	    _boa = theBOA;

	    return true;
	}
	else
	    return false;
    }


public static void addORBCleanup (ORBCleanup cleanup)
    {
	cleanups.put(cleanup, cleanup);
    }
 
    /**
     * Dispose of the object, i.e., unregister is from the ORB.
     */
 
public static void dispose (org.omg.CORBA.Object obj)
    {

	ORBInterface.orb().disconnect(obj);

    }

    /**
     * Register the object with the ORB.
     */
 
public static boolean objectIsReady (org.omg.CORBA.Object obj)
    {

	ORBInterface.orb().connect(obj);

	return true;
    }

public static boolean objectIsReady (org.omg.CORBA.Object obj, java.lang.Object[] params)
    {

	String name = (String) params[0];
	
	((com.ooc.CORBA.ORB) ORBInterface.orb()).connect(obj, name);
	
	return true;

    }    

    /**
     * Place the server into a state where it can begin to
     * accept requests for objects from clients.
     */
 
public static void run (String name)
    {



	_boa.impl_is_ready(null);



    }

public static void run ()
    {



	_boa.impl_is_ready(null);



    }

protected ORBInterface ()
    {
    }

private static void setORBInit (String[] params, boolean postInit)
    {
	Hashtable work = ((postInit) ? postORBInitProperty : preORBInitProperty);

	if (!work.isEmpty())
	{
	    Enumeration elements = work.elements();

	    while (elements.hasMoreElements())
	    {
		ORBInit p = (ORBInit) elements.nextElement();

		if (p != null)
		{
		    p.initialise(params);
		    p = null;
		}
	    }

	    work.clear();
	}
    }

private static void setBOAInit (String[] params, boolean preInit)
    {
	Hashtable work = ((preInit) ? preBOAInitProperty : postBOAInitProperty);

	if (!work.isEmpty())
	{
	    Enumeration elements = work.elements();

	    while (elements.hasMoreElements())
	    {
		BOAInit p = (BOAInit) elements.nextElement();

		if (p != null)
		{
		    p.initialise(params);
		    p = null;
		}
	    }

	    work.clear();
	}
    }    
    
private static org.omg.CORBA.ORB _orb = null;


private static org.omg.CORBA.BOA _boa = null;


private static Hashtable cleanups = new Hashtable();
private static Hashtable preORBInitProperty = new Hashtable();
private static Hashtable postORBInitProperty = new Hashtable();
private static Hashtable preBOAInitProperty = new Hashtable();
private static Hashtable postBOAInitProperty = new Hashtable();
private static ORBFilterManager filterManager = new ORBFilterManager();
    
    static
    {
	addORBInit(new LocalSetup());

    }

};
