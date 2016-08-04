/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PObjectUser.java,v 1.3 1998/04/09 15:47:11 nmcl Exp $
 */

import java.io.*;
import java.lang.NumberFormatException;

import com.arjuna.JTI.*;

public class PObjectUser
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
		System.out.println("Usage: PObjectUser -port <port> -host <host> [-uid <uid>] [-help]");
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
	
        PObject pObject = null;

        if (id != null)
	{
	    byte[] b = obj.get(tranID, s);

	    if ((b == null) || (b.length == 0))
	    {
	         System.out.println("Error getting bytes.");
		 obj.abort(tranID);
		 return;
	    }
            else
            {
                System.out.println("b length: " + b.length);

                try
	        {
                    ByteArrayInputStream bais = new ByteArrayInputStream(b);
                    ObjectInputStream    ois  = new ObjectInputStream(bais);

                    pObject = (PObject) ois.readObject();
		}
		catch (Exception e)
		{
                    System.out.println("ERROR: " + e);
                    e.printStackTrace(System.out);
		}
	    }
	}
        else
            pObject = new PObject();

        System.out.println("Value = " + pObject.getValue());

        pObject.setValue(pObject.getValue() + 1);

        System.out.println("Value increased locally");

        try
	{
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ObjectOutputStream    oos  = new ObjectOutputStream(baos);

            oos.writeObject(pObject);
            oos.flush();

            System.out.println("b length: " + baos.toByteArray().length);

            if (!obj.set(tranID, baos.toByteArray(), s))
	    {
		System.out.println("Could not save state.");
		
		obj.abort(tranID);
		return;
	    }
	}
        catch (Exception e)
	{
            System.out.println("ERROR: " + e);
            e.printStackTrace(System.out);
	}

	if (obj.commit(tranID))
	    System.out.println("Committed transaction: "+tranID);
	else
	    System.out.println("Could not commit transaction: "+tranID);

	if (obj.detach(s))
	    System.out.println("Removed state.");
	else
	    System.out.println("Could not remove state.");
    }
    
}
