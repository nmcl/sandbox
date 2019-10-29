/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: jacorb_1_2.java,v 1.1.2.1.4.1 2001/03/07 15:19:18 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

public class jacorb_1_2 extends ORBBase
{

public jacorb_1_2 ()
    {
	PropertyManager.setProperty("org.omg.CORBA.ORBClass", "jacorb.orb.ORB");
	PropertyManager.setProperty("org.omg.CORBA.ORBSingletonClass", "jacorb.orb.ORBSingleton");
	PropertyManager.setProperty("jacorb.retries", "5");
	PropertyManager.setProperty("jacorb.retry_interval", "500");
    }
    
}

