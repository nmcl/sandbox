/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SecurityTest.java,v 1.1 1998/11/18 09:01:36 nmcl Exp $
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
