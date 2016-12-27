/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistenceTest.java,v 1.1 1998/11/12 09:39:09 nmcl Exp $
 */

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaGandiva.Common.*;
import java.io.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import com.arjuna.JavaGandiva.Common.UidException;

public class PersistenceTest
{
    
public static void main (String[] args)
    {
	try
	{
	    ObjectStore store = new ObjectStore(JavaArjunaLiteNames.Implementation_ObjectStore_ShadowingStore());
	    OutputObjectState state = new OutputObjectState();
	    Uid u = new Uid();

	    System.out.println("Uid is "+u);

	    if (store.write_committed(u, "/StateManager/LockManager/foo", state))
		System.out.println("written ok");
	    else
		System.out.println("write error");
	}
	catch (ObjectStoreException e)
	{
	    System.out.println(e.getMessage());
	}
    }
    
}
