/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OAImple.java,v 1.1.2.2.4.1 2000/12/21 11:21:31 nmcl Exp $
 */

package com.arjuna.orb;

import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.SystemException;

/**
 * Some common methods for BOA and POA (Object Adapters).
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OAImple.java,v 1.1.2.2.4.1 2000/12/21 11:21:31 nmcl Exp $
 * @since JTS 2.1.
 */

public interface OAImple
{

    /**
     * Is this a Basic Object Adapter?
     */

public boolean supportsBOA ();

/**
 * Is this a Portable Object Adapter?
 */

public boolean supportsPOA ();

/**
 * Has the Object Adapter been initialised?
 */

public boolean initialised ();

/**
 * run is a way of starting a server listening for invocations.
 * For historical reasons we do this via the Object Adapter
 * interface, though POA ORBs may implement this via the ORB.
 */
 
public void run (com.arjuna.orb.ORB o, String name) throws SystemException;

/**
 * run is a way of starting a server listening for invocations.
 * For historical reasons we do this via the Object Adapter
 * interface, though POA ORBs may implement this via the ORB.
 */

public void run (com.arjuna.orb.ORB o) throws SystemException;

}
