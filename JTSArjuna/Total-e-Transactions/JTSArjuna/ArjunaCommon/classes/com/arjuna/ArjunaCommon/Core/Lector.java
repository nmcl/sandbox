/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Lector.java,v 1.1.2.1.2.1.2.1.2.2.4.2.2.1.16.3 2001/07/20 14:38:31 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Core;

import com.arjuna.ArjunaCommon.Common.*;

import java.net.UnknownHostException;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Lector.java,v 1.1.2.1.2.1.2.1.2.2.4.2.2.1.16.3 2001/07/20 14:38:31 nmcl Exp $
 * @since JTS 2.1.
 */

public class Lector
{

public static final synchronized boolean confirm (int value)
    {
	if (_li == null)
	    Lector.checkLicence();  // only need do once

	if ((value & _enabledCode) != 0) // component is enabled
	    return true;
	else
	    return false;
    }

private static final void checkLicence ()
    {
	_li = PropertyManager.getProperty("JAVA_ARJUNA_LICENCE");

	if (_li != null)
	{
	    int index1 = _li.indexOf('-');
	    int index2 = 0;
	    String hString = null;
	    String nmString = null;
	    String dString = null;
	    String eString = null;
	    String cString = null;
	    
	    boolean ok = true;
	    
	    if (index1 != -1)
		hString = _li.substring(0, index1);
	    else
		ok = false;

	    if (ok)
	    {
		index2 = _li.indexOf('-', index1 +1);
		    
		if (index2 != -1)
		{
		    nmString = _li.substring(index1 +1, index2);
		    index1 = index2;
		}
		else
		    ok = false;
	    }

	    if (ok)
	    {
		index2 = _li.indexOf('-', index1 +1);

		if (index2 != -1)
		{
		    dString = _li.substring(index1 +1, index2);
		    index1 = index2;
		}
		else
		    ok = false;
	    }

	    if (ok)
	    {
		index2 = _li.indexOf('-', index1 +1);

		if (index2 != -1)
		{
		    eString = _li.substring(index1 +1, index2);
		    index1 = index2;
		}
		else
		    ok = false;
	    }

	    if (ok)
	    {
		/*
		 * Check for any trailing spaces!
		 */

		index2 = _li.indexOf(' ', index1 +1);
		
		if (index2 == -1)
		    cString = _li.substring(index1 +1);
		else
		    cString = _li.substring(index1 +1, index2);
	    }

	    if (ok)
	    {
		int hostAddr = 0;
		int initTime = (int) (System.currentTimeMillis()/1000);

		try
		{
		    hostAddr = com.arjuna.ArjunaCommon.Common.Utility.hostInetAddr();	/* calculated only once */
		}
		catch (UnknownHostException e)
		{
		    e.printStackTrace();
		    
		    throw new FatalError(e.toString());
		}
		
		/*
		 * We have to use hexStringToLong rather than
		 * hexStringToInt because Java throws an exception for
		 * large (valid!) integer strings such as d0a15428.
		 */
	
		int suppliedHost = (int) Utility.hexStringToLong(hString);
		int suppliedNetmask = (int) Utility.hexStringToLong(nmString);
		int suppliedDate = (int) Utility.hexStringToLong(dString);
		int suppliedEnable = (int) Utility.hexStringToLong(eString);
		int suppliedCheck = (int) Utility.hexStringToLong(cString);

		int originalNetmask = suppliedNetmask ^ 0xfc451ade;
		int localHost = hostAddr ^ 0x4bf2abc4;
		int network = localHost & originalNetmask;
		int suppliedNetwork = suppliedHost & originalNetmask;
		int enabledCode = suppliedEnable;

		int calc = 0xa6a6a6a6;
		int a = suppliedDate;
		int b = network;
		int c = suppliedNetmask;
		int d = enabledCode;
		    
		for (int i = 0; i < 17; i++)
		{
		    a = (37 * (a ^ calc)) + 41;
		    b = (27 * (b ^ calc)) + 31;
		    c = (43 * (c ^ calc)) + 29;
		    d = (19 * (d ^ calc)) + 7;
		    calc = a ^ b ^ c ^ d;
		}

		suppliedDate = suppliedDate ^ 0xbeca54d2;
		    
		/*
		 * Check for 'infinite' timeout period.
		 */
		    
		if ((calc != suppliedCheck) || (suppliedNetwork != network) || ((suppliedDate <= initTime) && (suppliedDate != 0xffffffff)))
		{
		    System.err.println("Arjuna licence information is invalid. Please apply for a new licence.");

		    throw new com.arjuna.ArjunaCommon.Common.LicenceError("Arjuna licence information is invalid. Please apply for a new licence.");
		}

		_enabledCode = suppliedEnable ^ 0x1dc4fa42;
	    }
	    else
	    {
		System.err.println("Arjuna licence property variable has invalid format.");

		throw new com.arjuna.ArjunaCommon.Common.LicenceError("Arjuna licence property variable has invalid format.");
	    }
	}
	else
	{
	    System.err.println("Arjuna licence property variable JAVA_ARJUNA_LICENCE not found.");

	    throw new com.arjuna.ArjunaCommon.Common.LicenceError("Arjuna licence property variable JAVA_ARJUNA_LICENCE not found.");
	}
    }

private static String _li = null;
private static int    _enabledCode = 0;
    
}
