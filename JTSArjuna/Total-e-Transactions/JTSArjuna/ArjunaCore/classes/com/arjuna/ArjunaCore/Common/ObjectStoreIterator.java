/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreIterator.java,v 1.1.4.2 2000/10/20 13:10:16 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;

/**
 * Class that allows us to iterate through instances of objects
 * that may be stored within a specific object store.
 */

public class ObjectStoreIterator
{

public ObjectStoreIterator (ObjectStore store, String tName)
    {
	try
	{
	    store.allObjUids(tName, uidList);
	}
	catch (ObjectStoreException e)
	{
	}
    }
    
    /**
     * return the Uids from the list one at a time. ObjStore returns either
     * null list or a list terminated by the NIL_UID. Use the latter to return
     * 0 (for end of list)
     */

public final synchronized Uid iterate ()
    {
	Uid newUid = null;
	
	try
	{
	    newUid = new Uid(Uid.nullUid());

	    newUid.unpack(uidList);
	}
	catch (IOException e)
	{
	}

	return newUid;
    }

private InputObjectState uidList;
    
}
    
