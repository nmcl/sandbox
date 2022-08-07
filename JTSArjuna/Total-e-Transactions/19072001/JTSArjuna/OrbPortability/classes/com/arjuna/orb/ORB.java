/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ORB.javatmpl,v 1.1.2.4.2.2.2.1.2.1.4.1.16.1 2001/06/15 12:15:49 nmcl Exp $
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
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

/**
 * An instance of this class provides access to the ORB specific
 * ORB implementation object.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORB.javatmpl,v 1.1.2.4.2.2.2.1.2.1.4.1.16.1 2001/06/15 12:15:49 nmcl Exp $
 * @since JTS 2.1.
 */

public class ORB
{

public boolean initialised ()
    {
	return _theORB.initialised();
    }
    
public void init () throws SystemException
    {
	_theORB.init();
    }
    
public void init (Applet a, Properties p) throws SystemException
    {
	_theORB.init(a, p);
    }
    
public void init (String[] s, Properties p) throws SystemException
    {
	_theORB.init(s, p);
    }

public void shutdown () throws SystemException
    {
	_theORB.shutdown();
    }

public void destroy () throws SystemException
    {
	_theORB.destroy();
    }

public org.omg.CORBA.ORB orb () throws SystemException
    {
	return _theORB.orb();
    }

public void orb (org.omg.CORBA.ORB o) throws SystemException
    {
	_theORB.orb(o);
    }
    
private static ORBImple _theORB;
    
    static
    {
	/*
	 * Let the application provide its own ORB implementation.
	 */

	String className = PropertyManager.getProperty(com.arjuna.OrbCommon.ORBEnvironment.ORB_IMPLEMENTATION);
	
	if (className == null)
	{






	    className = "com.arjuna.orb.implementations.ORB.orbix2000_";







	    className = className+ "1_2" ;
	}

	try
	{
	    Class c = Class.forName(className);
	
	    _theORB = (ORBImple) c.newInstance();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("ORB specific class creation failed with: "+e);

	    throw new ExceptionInInitializerError("ORB specific class creation failed with: "+e);
	}
    }
 
}

