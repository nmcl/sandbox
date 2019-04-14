/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStateQuery.java,v 1.1 2003/06/19 10:50:27 nmcl Exp $
 */

package com.arjuna.ats.arjuna.tools;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import java.io.PrintWriter;

public class ObjectStateQuery
{
    
public static void main (String[] args)
    {
	String uid = null;
	String type = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		usage();
		System.exit(0);
	    }
	    else
	    {
		if (args[i].compareTo("-uid") == 0)
		{
		    uid = args[i+1];
		    i++;
		}
		else
		{
		    if (args[i].compareTo("-type") == 0)
		    {
			type = args[i+1];
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
	    ObjectStore imple = new ObjectStore();

	    System.out.println("Status is "+imple.currentState(new Uid(uid), type));

	    InputObjectState buff = new InputObjectState();
	    
	    imple.allObjUids(type, buff, ObjectStore.OS_UNCOMMITTED);
	    
	    Uid u = new Uid(Uid.nullUid());
	    
	    u.unpack(buff);
	    
	    System.out.println("got "+u);
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	}
    }

private static void usage ()
    {
	System.out.println("Usage: ObjectStateQuery [-uid <state id>] [-type <state type>] [-help]");
    }
 
}

