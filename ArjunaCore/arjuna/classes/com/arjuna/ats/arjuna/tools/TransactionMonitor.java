/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TransactionMonitor.java,v 1.2 2003/08/11 14:07:39 nmcl Exp $
 */

package com.arjuna.ats.arjuna.tools;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import java.io.PrintWriter;

public class TransactionMonitor
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

	/* Determine transaction (BasicAction) type name */
	BasicAction ba = new BasicAction();
	String baType = ba.type();
	if (baType.charAt(0) == '/') 
		baType = baType.substring(1);
	try
	{
	    TxControl txc = new TxControl();
	    ObjectStore imple = txc.getStore();

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
			else if (theName.startsWith(baType))
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
	System.out.println("Usage: TransactionMonitor [-root <store root>] [-help]");
    }
 
};

