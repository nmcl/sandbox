/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionMonitor.java,v 1.1.4.1 2000/10/16 16:00:05 nmcl Exp $
 */

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.PrintWriter;

public class ActionMonitor
{
    
public static void main (String[] args)
    {
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

	try
	{
	    ClassName actionStoreType = new ClassName(PropertyManager.getProperty(ArjunaCoreEnvironment.ACTION_STORE, ArjunaCoreNames.Implementation_ObjectStore_defaultActionStore().stringForm()));

	    ObjectStore imple = new ObjectStore(actionStoreType, root);
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
 					    ObjectStore.printStateStatus(new PrintWriter(System.out), imple.currentState(theUid, theName));
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

