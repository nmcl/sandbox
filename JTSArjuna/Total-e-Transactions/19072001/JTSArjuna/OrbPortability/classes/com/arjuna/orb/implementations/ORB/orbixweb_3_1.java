/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: orbixweb_3_1.java,v 1.1.2.2.4.1 2000/12/14 15:10:53 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.orb.ORBImple;
import IE.Iona.OrbixWeb._CORBA;
import IE.Iona.OrbixWeb._OrbixWeb;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

public class orbixweb_3_1 extends ORBBase
{

public orbix2000_1_1 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "IE.Iona.OrbixWeb.CORBA.ORB", true);
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "IE.Iona.OrbixWeb.CORBA.ORB", true);
    }

public synchronized void shutdown () throws SystemException
    {
	_CORBA.Orbix.shutdown(false); // OrbixWeb ignores the parameter!
	_init = false;
    }

public synchronized void destroy () throws SystemException
    {
	shutdown();
    }
 
    static
    {
	ORBInterface.addORBAttribute(new com.arjuna.OrbCommon.Filters.orbixweb.Init());
    }
    
}
