/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: sunsjdk_1_3.java,v 1.1.2.2 2000/11/14 11:55:17 nmcl Exp $
 */

package com.arjuna.orb.implementations.ORB;

import com.arjuna.orb.ORBImple;
import java.util.*;
import java.applet.Applet;
import java.io.*;

import org.omg.CORBA.SystemException;

public class sunsjdk_1_3 extends ORBBase
{

public synchronized void shutdown () throws SystemException
    {
	_init = false;
    }

public synchronized void destroy () throws SystemException
    {
	shutdown();
    }

}
