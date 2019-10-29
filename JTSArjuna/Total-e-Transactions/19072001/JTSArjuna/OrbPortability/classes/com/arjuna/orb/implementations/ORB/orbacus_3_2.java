/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbacus_3_2.java,v 1.1.2.2.4.1 2000/12/14 15:10:52 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

public class orbacus_3_2 extends ORBBase
{

public orbacus_3_2 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "com.ooc.CORBA.ORBSingleton", true);
    }

public synchronized void shutdown () throws SystemException
    {
	_init = false;
    }

public synchronized void destroy () throws SystemException
    {
	shutdown();
    }

}

