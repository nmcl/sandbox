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

import com.arjuna.JavaArjuna.ObjectStore.ObjectStore;
import com.arjuna.JavaArjuna.ObjectStore.ObjectStoreType;
import com.arjuna.JavaArjuna.Common.*;
import java.io.*;

import com.arjuna.JavaArjuna.Common.ObjectStoreException;
import com.arjuna.JavaArjuna.Common.UidException;

public class PersistenceTest
{
    
public static void main (String[] args)
    {
	try
	{
	    ObjectStore store = new ObjectStore(ObjectStoreType.SHADOWING_STORE);
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
