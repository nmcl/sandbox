/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ORBImple.java,v 1.1.2.2.4.1 2000/12/21 11:21:32 nmcl Exp $
 */

package com.arjuna.orb;

import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

/*
 * Some ORBs either don't support a shutdown operation or
 * don't have one that waits until all outstanding references
 * to the OA/ORB have been released. So, we can't simply
 * null out our reference when our shutdown methods are called.
 * This is more of an issue for multi-threaded applications, where
 * one thread "shuts" the system down, while other threads are still
 * active. So, we have state variables to allow us to tell whether
 * the system is supposed to be shutdown. This then allows us to
 * support multiple initialisations.
 */

/**
 * The Object Request Broker interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ORBImple.java,v 1.1.2.2.4.1 2000/12/21 11:21:32 nmcl Exp $
 * @since JTS 2.1.
 */

public interface ORBImple
{

    /**
     * Is the ORB initialised?
     */

public boolean initialised ();

/**
 * Initialise the ORB.
 */
 
public void init () throws SystemException;
public void init (Applet a, Properties p) throws SystemException;
public void init (String[] s, Properties p) throws SystemException;

/**
 * Shutdown the ORB.
 */

public void shutdown () throws SystemException;

/**
 * Destroy the ORB.
 */

public void destroy () throws SystemException;

/**
 * Return a reference to the ORB.
 */

public org.omg.CORBA.ORB orb () throws SystemException;

/**
 * Provide a reference to the ORB. Used if the application must
 * initialise the ORB separately.
 */

public void orb (org.omg.CORBA.ORB o) throws SystemException;

}
