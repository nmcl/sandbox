/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: POAImple.java,v 1.1.2.1.2.2 2001/01/04 12:22:41 nmcl Exp $
 */

package com.arjuna.orb;

import org.omg.PortableServer.*;
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
 * The Portable Object Adapter interface.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: POAImple.java,v 1.1.2.1.2.2 2001/01/04 12:22:41 nmcl Exp $
 * @since JTS 2.1.
 */

public interface POAImple extends OAImple
{

    /**
     * Initialise the root POA.
     */

public void init (com.arjuna.orb.ORB o) throws InvalidName, AdapterInactive, SystemException;

/**
 * Create a child POA of the root POA.
 */

public void createPOA (String adapterName,
		       Policy[] policies) throws AdapterAlreadyExists, InvalidPolicy, AdapterInactive, SystemException;

/**
 * Destroy the root POA.
 */

public void destroyRootPOA () throws SystemException;

/**
 * Destroy the child POA.
 */

public void destroyPOA (String adapterName) throws SystemException;

/**
 * @return a reference to the root POA.
 */

public org.omg.PortableServer.POA rootPoa () throws SystemException;

/**
 * Provide a reference to the root POA. Used if the application must
 * initialise the POA separately.
 */

public void rootPoa (org.omg.PortableServer.POA thePOA) throws SystemException;

/**
 * @return a reference to the child POA.
 */
 
public org.omg.PortableServer.POA poa (String adapterName) throws SystemException;

/**
 * Provide a reference to the child POA. Used if the application must
 * initialise the POA separately.
 */

public void poa (String adapterName, org.omg.PortableServer.POA thePOA) throws SystemException;
 
}
