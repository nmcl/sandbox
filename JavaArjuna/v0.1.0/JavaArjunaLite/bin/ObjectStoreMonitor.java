/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectStoreMonitor.java,v 1.1 1998/07/31 15:15:22 nmcl Exp $
 */

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaArjunaLite.Common.*;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaGandiva.Common.*;

public class ActionMonitor
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
 					    ObjectStore.printState(System.out, imple.currentState(theUid, theName));
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

