package com.hp.mwtests.ts.arjuna.security;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: SecurityTest.java,v 1.1 2002/04/18 15:24:01 rbegg Exp $
 */

public class SecurityTest
{
    
public static void main (String[] args)
    {
	SecurityManager securityManager = System.getSecurityManager();

	if (securityManager == null)
	{
	    /*
	     * Means we were loaded locally, so no security manager.
	     */
	     
	    System.out.println("Could not get SecurityManager.");
	    System.exit(0);
	}
	
	try
	{
	    securityManager.checkRead("/etc/passwd");

	    System.out.println("Can read.");
	}
	catch (SecurityException e)
	{
	    System.out.println("Cannot read.");
	}

	try
	{
	    securityManager.checkWrite("/tmp/foo");

	    System.out.println("Can write.");
	}
	catch (SecurityException e)
	{
	    System.out.println("Cannot write.");
	}
    }
    
}
