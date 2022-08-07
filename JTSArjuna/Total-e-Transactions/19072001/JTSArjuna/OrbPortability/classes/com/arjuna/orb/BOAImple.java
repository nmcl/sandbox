/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BOAImple.java,v 1.1.2.2.2.1.2.1 2000/12/21 11:21:31 nmcl Exp $
 */

package com.arjuna.orb;

import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

/**
 * The Basic Object Adapter interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BOAImple.java,v 1.1.2.2.2.1.2.1 2000/12/21 11:21:31 nmcl Exp $
 * @since JTS 2.1.
 */

public interface BOAImple extends OAImple
{

    /**
     * Initialise the BOA.
     */

public void init (com.arjuna.orb.ORB o) throws InvalidName, SystemException;

    /**
     * Initialise the BOA with the provided parameters.
     */

public void init (com.arjuna.orb.ORB o, String s,
		  Properties p) throws InvalidName, SystemException;

    /**
     * Initialise the BOA with the provided parameters.
     */

public void init (com.arjuna.orb.ORB o, String[] s,
		  Properties p) throws InvalidName, SystemException;

/**
 * Shutdown the BOA.
 */

public void shutdown (String name) throws SystemException;

/**
 * Destroy the BOA.
 */

public void destroy () throws SystemException;

/**
 * Return a reference to the BOA. Because of compatibility issues,
 * the caller must cast this to the right type for the ORB.
 */

public java.lang.Object boa () throws SystemException;

/**
 * Provide a reference to the BOA. Used if the application must
 * initialise the BOA separately.
 */

public void boa (java.lang.Object o) throws SystemException;

}
