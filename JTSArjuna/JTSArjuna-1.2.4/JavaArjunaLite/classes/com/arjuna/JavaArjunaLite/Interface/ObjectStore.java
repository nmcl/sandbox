/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectStore.java,v 1.7 1998/07/06 13:26:52 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Interface;

import com.arjuna.JavaArjunaLite.Implementation.ObjectStoreImple;
import com.arjuna.JavaArjunaLite.Implementation.Implementations;
import com.arjuna.JavaGandiva.Interface.Inventory;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import java.io.PrintStream;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import java.io.IOException;

public class ObjectStore
{

    /*
     * StateStatus
     */
    
public static final int OS_UNKNOWN = 0;
public static final int OS_COMMITTED = 1;
public static final int OS_UNCOMMITTED = 2;
public static final int OS_HIDDEN = 4;
public static final int OS_COMMITTED_HIDDEN = ObjectStore.OS_COMMITTED | ObjectStore.OS_HIDDEN;
public static final int OS_UNCOMMITTED_HIDDEN = ObjectStore.OS_UNCOMMITTED | ObjectStore.OS_HIDDEN;

    /*
     * StateType
     */
    
public static final int OS_SHADOW = 0;
public static final int OS_ORIGINAL = 1;
public static final int OS_INVISIBLE = 2;

public ObjectStore (String location)
    {
	/*
	 * Check once per application. At present this means
	 * that all objects have the same object store implementation.
	 * However, this need not be the case, and could be an
	 * attribute of the object, or derived from the object's name.
	 */

	if (objectStoreType == null)
	    objectStoreType = new ClassName(System.getProperty(JavaArjunaLiteNames.Interface_ObjectStore_storeType(), JavaArjunaLiteNames.Implementation_ObjectStore_defaultStore().stringForm()));

	Object[] param = new Object[1];
	param[0] = location;
	
	Object ptr = Inventory.inventory().createResources(objectStoreType, param);

	param = null;
	
	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }
    
public ObjectStore (ClassName typeName)
    {
	Object ptr = Inventory.inventory().createVoid(typeName);

	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }
    
public ObjectStore (ClassName typeName, String location)
    {
	Object[] param = new Object[1];
	param[0] = location;
	
	Object ptr = Inventory.inventory().createResources(typeName, param);

	param = null;
	
	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }

public void finalize ()
    {
	_imple = null;
    }

public synchronized boolean allObjUids (String s, InputObjectState buff, int m) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.allObjUids(s, buff, m) : false);
    }
    
public synchronized boolean allObjUids (String s, InputObjectState buff) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.allObjUids(s, buff, ObjectStore.OS_UNKNOWN) : false);
    }
  
public synchronized boolean allTypes (InputObjectState buff) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.allTypes(buff) : false);
    }
    
public synchronized int currentState (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.currentState(u, tn) : ObjectStore.OS_UNKNOWN);
    }
    
public String getStoreName ()
    {
	return ((_imple != null) ? _imple.getStoreName() : null);
    }
    
public synchronized boolean commit_state (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.commit_state(u, tn) : false);
    }
    
public synchronized boolean hide_state (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.hide_state(u, tn) : false);
    }
    
public synchronized boolean reveal_state (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.reveal_state(u, tn) : false);
    }
    
public synchronized InputObjectState read_committed (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.read_committed(u, tn) : null);
    }
    
public synchronized InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.read_uncommitted(u, tn) : null);
    }
    
public synchronized boolean remove_committed (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.remove_committed(u, tn) : false);
    }
    
public synchronized boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.remove_uncommitted(u, tn) : false);
    }
    
public synchronized boolean write_committed (Uid u, String tn, OutputObjectState buff) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.write_committed(u, tn, buff) : false);
    }
    
public synchronized boolean write_uncommitted (Uid u, String tn, OutputObjectState buff) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.write_uncommitted(u, tn, buff) : false);
    }
    
public ClassName className ()
    {
	return ((_imple != null) ? _imple.className() : ClassName.invalid());
    }

public String locateStore (String location) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.locateStore(location) : null);
    }

public boolean fullCommitNeeded ()
    {
	return ((_imple != null) ? _imple.fullCommitNeeded() : false);
    }

public synchronized boolean isType (Uid u, String tn, int st) throws ObjectStoreException
    {
	return ((_imple != null) ? _imple.isType(u, tn, st) : false);
    }

public synchronized void initialise (Uid u, String tn)
    {
	if (_imple != null)
	    _imple.initialise(u, tn);
    }
    
public synchronized void storeLocation (String[] s)
    {
	if (_imple != null)
	    _imple.storeLocation(s);
    }

public synchronized void useStoreLocation (boolean b)
    {
	if (_imple != null)
	    _imple.useStoreLocation(b);
    }
    
public synchronized void setObjectData (Uid u, String tn)
    {
	if (_imple != null)
	    _imple.setObjectData(u, tn);
    }

public synchronized void pack (OutputBuffer buff) throws IOException
    {
	if (_imple != null)
	    _imple.pack(buff);
	else
	    throw(new IOException("No implementation!"));
    }
    
public synchronized void unpack (InputBuffer buff) throws IOException
    {
	if (_imple != null)
	    _imple.unpack(buff);
	else
	    throw(new IOException("No implementation!"));
    }

public static void printState (PrintStream strm, int res)
    {
	switch (res)
	{
	case ObjectStore.OS_UNKNOWN:
	    strm.print("ObjectStore.OS_UNKNOWN");
	    break;
	case ObjectStore.OS_COMMITTED:
	    strm.print("ObjectStore.OS_COMMITTED");
	    break;
	case ObjectStore.OS_UNCOMMITTED:
	    strm.print("ObjectStore.OS_UNCOMMITTED");
	    break;
	case ObjectStore.OS_HIDDEN:
	    strm.print("ObjectStore.OS_HIDDEN");
	    break;
	case ObjectStore.OS_COMMITTED_HIDDEN:
	    strm.print("ObjectStore.OS_COMMITTED_HIDDEN");
	    break;
	case ObjectStore.OS_UNCOMMITTED_HIDDEN:
	    strm.print("ObjectStore.OS_UNCOMMITTED_HIDDEN");
	    break;
	}
    }

private ObjectStoreImple _imple;

private static ClassName objectStoreType = null;
    
    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }
    
}

