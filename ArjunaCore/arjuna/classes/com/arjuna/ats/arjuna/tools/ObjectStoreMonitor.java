/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreMonitor.java,v 1.1 2003/06/19 10:50:27 nmcl Exp $
 */

package com.arjuna.ats.arjuna.tools;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import java.io.PrintWriter;

public class ObjectStoreMonitor
{
    
public static void main (String[] args)
    {
	String storeImple = null;
	String root = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		usage();
		System.exit(0);
	    }
	    else
	    {
		if (args[i].compareTo("-store") == 0)
		{
		    storeImple = args[i+1];
		    i++;
		}
		else
		{
		    if (args[i].compareTo("-root") == 0)
		    {
			root = args[i+1];
			i++;
		    }
		    else
		    {
			System.out.println("Unknown option "+args[i]);
			usage();

			System.exit(0);
		    }
		}
	    }
	}

	try
	{
	    ObjectStore imple = null;

	    if (storeImple != null)
		imple = new ObjectStore(new ClassName(storeImple), root);
	    else
		imple = new ObjectStore(root);
	    
	    InputObjectState types = new InputObjectState();

	    if (imple.allTypes(types))
	    {
		String theName = null;
		int count = 0;

		try
		{
		    boolean endOfList = false;

		    while (!endOfList)
		    {
			theName = types.unpackString();

			if (theName.compareTo("") == 0)
			    endOfList = true;
			else
			{
			    count++;
	    
			    System.out.println(count+": "+theName);

			    InputObjectState uids = new InputObjectState();

			    if (imple.allObjUids(theName, uids))
			    {
				Uid theUid = new Uid(Uid.nullUid());

				try
				{
				    boolean endOfUids = false;
				    
				    while (!endOfUids)
				    {
					theUid.unpack(uids);

					if (theUid.equals(Uid.nullUid()))
					    endOfUids = true;
					else
					{
					    System.out.print("\t"+theUid+" state is ");
					    System.out.print(ObjectStore.stateStatusString(imple.currentState(theUid, theName)));

					    System.out.println();
					}
				    }
				}
				catch (Exception e)
				{
				    // end of uids!
				}
			    }

			    System.out.println();
			}
		    }
		}
		catch (Exception e)
		{
		    System.err.println(e);
		    
		    // end of list!
		}
	    }
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	}
    }

private static void usage ()
    {
	System.out.println("Usage: ActionMonitor [-store <object store>] [-root <store root>] [-help]");
    }
 
};

