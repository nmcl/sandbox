/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PersistenceTest.java,v 1.1 2000/02/25 14:04:43 nmcl Exp $
 */

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
import com.arjuna.ArjunaCommon.Common.UidException;

public class PersistenceTest
{
    
public static void main (String[] args)
    {
	try
	{
	    ObjectStore store = new ObjectStore(ArjunaLiteNames.Implementation_ObjectStore_ShadowingStore());
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
