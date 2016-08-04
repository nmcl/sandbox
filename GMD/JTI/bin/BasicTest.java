/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicTest.java,v 1.2 1998/04/09 15:47:10 nmcl Exp $
 */

import com.arjuna.JTI.*;

import java.lang.NumberFormatException;

public class BasicTest
{
    
public static void main (String[] args)
    {
	int portNumber = 0;
	String hostName = null;
	String id = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-port") == 0)
	    {
		try
		{
		    portNumber = Integer.valueOf(args[i+1]).intValue();
		}
		catch (NumberFormatException e)
		{
		    System.err.println("Invalid port number "+args[i+1]);

		    return;
		}
	    }
	    if (args[i].compareTo("-host") == 0)
		hostName = args[i+1];
	    if (args[i].compareTo("-uid") == 0)
		id = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: BasicTest -port <port> -host <host> [-uid <uid>] [-help]");
		return;
	    }
	}

	if (portNumber == 0)
	{
	    System.err.println("Error - no port number specified.");
	    return;
	}

	if (hostName == null)
	{
	    System.err.println("Error - no host name specified.");
	    return;
	}

	ObjectStore obj = new ObjectStore(hostName, portNumber);
	String s = null;

	if (id == null)
	{
	    s = obj.create();
	    System.out.println("Created "+s);
	}
	else
	{
	    s = id;
	    obj.attach(s);
	    System.out.println("Using existing object "+s);
	}

	String tranID = obj.begin();

	System.out.println("Begun transaction: "+tranID);
	
	byte[] b = obj.get(tranID, s);

	if (b == null)
	{
	    System.out.println("Error getting bytes.");
	}

	String foo = new String("hello world");

	b = foo.getBytes();

	if (obj.set(tranID, b, s))
	{
	    System.out.println("Set state ok.");

	    b = obj.get(tranID, s);

	    if (b == null)
	    {
		System.out.println("Error getting bytes back.");
	    }
	    else
	    {
		String bar = new String(b);

		System.out.println("Got back "+bar);
	    }
	}
	else
	{
	    System.out.println("Error setting state.");
	}

	foo = new String("This is a test");
	
	if (obj.set(tranID, foo.getBytes(), s))
	{
	    b = obj.get(tranID, s);

	    if (b == null)
	    {
		System.out.println("Error getting bytes back.");
	    }
	    else
	    {
		String bar = new String(b);

		System.out.println("Got back "+bar);
	    }
	}
	else
	    System.out.println("Error setting state.");

	if (obj.commit(tranID))
	    System.out.println("Committed transaction: "+tranID);
	else
	    System.out.println("Could not commit transaction: "+tranID);

	obj.detach(s);
    }
    
}
