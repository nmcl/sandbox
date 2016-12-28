/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreIterator.java,v 1.1 2000/02/25 14:04:00 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Common;

import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
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
    
