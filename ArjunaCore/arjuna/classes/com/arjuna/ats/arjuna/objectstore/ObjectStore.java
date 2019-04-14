/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStore.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 */

package com.arjuna.ats.arjuna.objectstore;

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;
import com.arjuna.ats.arjuna.gandiva.nameservice.NameService;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.internal.arjuna.Implementations;
import java.io.PrintWriter;
import com.arjuna.ats.arjuna.logging.tsLogger;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import java.io.IOException;

/**
 * The object store interface is the application's route to using
 * a specific object store implementation. The interface dynamically
 * binds to an implementation of the right type.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStore.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 * @since JTS 1.0.
 */

public class ObjectStore
{

    /**
     * StateStatus
     */
    
public static final int OS_UNKNOWN = 0;  // means no state present.
public static final int OS_COMMITTED = 1;
public static final int OS_UNCOMMITTED = 2;
public static final int OS_HIDDEN = 4;
public static final int OS_COMMITTED_HIDDEN = ObjectStore.OS_COMMITTED | ObjectStore.OS_HIDDEN;
public static final int OS_UNCOMMITTED_HIDDEN = ObjectStore.OS_UNCOMMITTED | ObjectStore.OS_HIDDEN;

    /**
     * StateType.
     */
    
public static final int OS_SHADOW = 10;
public static final int OS_ORIGINAL = 11;
public static final int OS_INVISIBLE = 12;

public static final int OS_SHARED = 13;
public static final int OS_UNSHARED = 14;
    
public ObjectStore ()
    {
	this("", ObjectStore.OS_UNSHARED);
    }

public ObjectStore (int shareStatus)
    {
	this("", shareStatus);
    }
    
public ObjectStore (String location)
    {
	this(location, ObjectStore.OS_UNSHARED);
    }
    
public ObjectStore (String location, int shareStatus)
    {
	if (objectStoreType == null)
	    getDefault();
	
	Object[] param = new Object[2];
	param[0] = location;
	param[1] = new Integer(shareStatus);
	
	Object ptr = Inventory.inventory().createResources(objectStoreType, param);

	param = null;
	
	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }
    
    /**
     * Defaults to OS_UNSHARED mode.
     */

public ObjectStore (ClassName typeName)
    {
	if (typeName == null)
	    typeName = getDefault();
	
	Object ptr = Inventory.inventory().createVoid(typeName);

	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }
    
public ObjectStore (ClassName typeName, int shareStatus)
    {
	this(typeName, "", shareStatus);
    }
    
public ObjectStore (ClassName typeName, String location)
    {
	this(typeName, location, ObjectStore.OS_UNSHARED);
    }
    
public ObjectStore (ClassName typeName, String location, int shareStatus)
    {
	if (typeName == null)
	    typeName = getDefault();
	
	Object[] param = new Object[2];
	param[0] = location;
	param[1] = new Integer(shareStatus);
	
	Object ptr = Inventory.inventory().createResources(typeName, param);

	param = null;
	
	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;
    }

public ObjectStore (ClassName typeName, ObjectName objName)
    {
	if (typeName == null)
	    typeName = getDefault();
	
	Object ptr = Inventory.inventory().createObjectName(typeName, objName);
	
	if (ptr instanceof ObjectStoreImple)
	    _imple = (ObjectStoreImple) ptr;
	else
	    _imple = null;	
    }

public ObjectStore (ObjectName objName)
    {
	ClassName typeName = null;
	
	try
	{
	    typeName = objName.getClassNameAttribute(Environment.OBJECTSTORE_TYPE);
	}
	catch (Exception ex)
	{
	}
	
	if (typeName == null)
	    typeName = getDefault();
	
	ObjectName osObjName = null;
	
	try
	{
	    osObjName = objName.getObjectNameAttribute(ArjunaNames.ObjectStore_implementationObjectName());
	}
	catch (Exception ex)
	{
	}

	if (osObjName == null)
	    osObjName = objName;
	
	Object ptr = Inventory.inventory().createObjectName(typeName, osObjName);
	
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

public int typeIs ()
    {
	return ((_imple != null) ? _imple.typeIs() : -1);
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

    /**
     * @message com.arjuna.ats.arjuna.objectstore.ObjectStore_1 [com.arjuna.ats.arjuna.objectstore.ObjectStore_1] - No implmentation!
     */
    
public synchronized void pack (OutputBuffer buff) throws IOException
    {
	if (_imple != null)
	    _imple.pack(buff);
	else
	    throw new IOException(tsLogger.log_mesg.getString("com.arjuna.ats.arjuna.objectstore.ObjectStore_1"));
    }
    
public synchronized void unpack (InputBuffer buff) throws IOException
    {
	if (_imple != null)
	    _imple.unpack(buff);
	else
	    throw new IOException(tsLogger.log_mesg.getString("com.arjuna.ats.arjuna.objectstore.ObjectStore_1"));
    }

public static void printStateStatus (PrintWriter strm, int res)
    {
	strm.print(stateStatusString(res));
    }

public static String stateStatusString (int res)
    {
	switch (res)
	{
	case ObjectStore.OS_UNKNOWN:
	    return "ObjectStore.OS_UNKNOWN";
	case ObjectStore.OS_COMMITTED:
	    return "ObjectStore.OS_COMMITTED";
	case ObjectStore.OS_UNCOMMITTED:
	    return "ObjectStore.OS_UNCOMMITTED";
	case ObjectStore.OS_HIDDEN:
	    return "ObjectStore.OS_HIDDEN";
	case ObjectStore.OS_COMMITTED_HIDDEN:
	    return "ObjectStore.OS_COMMITTED_HIDDEN";
	case ObjectStore.OS_UNCOMMITTED_HIDDEN:
	    return "ObjectStore.OS_UNCOMMITTED_HIDDEN";
	default:
	    return "Unknown";
	}
    }

public static void printStateType (PrintWriter strm, int res)
    {
	strm.print(stateTypeString(res));
    }

public static String stateTypeString (int res)
    {
	switch (res)
	{
	case ObjectStore.OS_SHADOW:
	    return "ObjectStore.OS_SHADOW";
	case ObjectStore.OS_ORIGINAL:
	    return "ObjectStore.OS_ORIGINAL";
	case ObjectStore.OS_INVISIBLE:
	    return "ObjectStore.OS_INVISIBLE";
	default:
	    return "Unknown";
	}
    }

public final int shareState ()
    {
	return ((_imple != null) ? _imple.shareState() : -1);
    }
    
public final String storeDir ()
    {
	return ((_imple != null) ? _imple.storeDir() : null);
    }
    
public final String storeRoot ()
    {
	return ((_imple != null) ? _imple.storeRoot() : null);
    }

    /**
     * @return the default user store to use.
     * @see com.arjuna.ats.arjuna.gandiva.ClassName
     */

private ClassName getDefault ()
    {
	/*
	 * Check once per application. At present this means
	 * that all objects have the same object store implementation.
	 * However, this need not be the case, and could be an
	 * attribute of the object, or derived from the object's name.
	 */

	if (objectStoreType == null)
	    objectStoreType = new ClassName(arjPropertyManager.propertyManager.getProperty(Environment.OBJECTSTORE_TYPE, ArjunaNames.Implementation_ObjectStore_defaultStore().stringForm()));
	
	return objectStoreType;
    }
	    
private ObjectStoreImple _imple;

private static ClassName objectStoreType = null;
    
    static 
    {
	if (!Implementations.added())
	    Implementations.initialise();
    }
    
}

