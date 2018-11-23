/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectStoreIterator.java,v 1.4 1998/07/06 13:26:36 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import com.arjuna.JavaArjuna.ObjectStore.ObjectStore;
import com.arjuna.JavaArjuna.Common.*;

import com.arjuna.JavaArjuna.Common.ObjectStoreException;
import java.io.IOException;

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
    
    /*
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
    
