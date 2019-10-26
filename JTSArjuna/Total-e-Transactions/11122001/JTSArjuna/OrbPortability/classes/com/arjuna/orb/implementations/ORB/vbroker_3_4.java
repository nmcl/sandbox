/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: vbroker_3_4.java,v 1.1.2.3.4.1 2000/12/14 15:10:53 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

public class vbroker_3_4 extends ORBBase
{

public vbroker_3_4 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "com.visigenic.vbroker.orb.ORB", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "com.visigenic.vbroker.orb.ORB", true);
    }

public synchronized void shutdown () throws SystemException
    {
	((com.visigenic.vbroker.orb.ORB) _orb).shutdown();
	_init = false;
    }

public synchronized void destroy () throws SystemException
    {
	shutdown();
    }
 
}

