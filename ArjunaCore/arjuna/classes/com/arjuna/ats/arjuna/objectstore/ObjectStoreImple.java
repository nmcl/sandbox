/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreImple.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 */

package com.arjuna.ats.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.common.*;

import java.io.File;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import java.io.IOException;

/**
 * This is the base class from which all object store types are derived.
 * Note that because object store instances are stateless, to improve
 * efficiency we try to only create one instance of each type per process.
 * Therefore, the create and destroy methods are used instead of new
 * and delete. If an object store is accessed via create it *must* be
 * deleted using destroy. Of course it is still possible to make use of
 * new directly.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreImple.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class ObjectStoreImple
{

    /**
     * The type of the object store.
     */

public abstract int typeIs ();

    /**
     * @return all object Uids of the specified type.
     */

public abstract boolean allObjUids (String s, InputObjectState buff, int m) throws ObjectStoreException;

    /**
     * @return all types of objects stored in the object store.
     */

public abstract boolean allTypes (InputObjectState buff) throws ObjectStoreException;

    /**
     * @return the current state of the object's state (e.g., shadowed,
     * committed ...)
     */

public abstract int currentState (Uid u, String tn) throws ObjectStoreException;

    /**
     * @return the name of the object store.
     */

public abstract String getStoreName ();

    /**
     * Commit the object's state in the object store.
     */

public abstract boolean commit_state (Uid u, String tn) throws ObjectStoreException;

    /**
     * Hide the object's state in the object store. Used by crash
     * recovery.
     */

public abstract boolean hide_state (Uid u, String tn) throws ObjectStoreException;

    /**
     * Reveal a hidden object's state.
     */

public abstract boolean reveal_state (Uid u, String tn) throws ObjectStoreException;

    /**
     * Read the object's committed state.
     */

public abstract InputObjectState read_committed (Uid u, String tn) throws ObjectStoreException;
    
    /**
     * Read the object's shadowed state.
     */

public abstract InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException;

    /**
     * Remove the object's committed state.
     */

public abstract boolean remove_committed (Uid u, String tn) throws ObjectStoreException;

    /**
     * Remove the object's uncommitted state.
     */

public abstract boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException;

    /**
     * Write a new copy of the object's committed state.
     */

public abstract boolean write_committed (Uid u, String tn, OutputObjectState buff) throws ObjectStoreException;

    /**
     * Write a copy of the object's uncommitted state.
     */

public abstract boolean write_uncommitted (Uid u, String tn, OutputObjectState buff) throws ObjectStoreException;

    /**
     * @return the full path of the store. localOSRoot is always a relative
     * name.
     *
     * NOTE this path always ends in a /
     */

public String locateStore (String localOSRoot) throws ObjectStoreException
    {
	if (_objectStoreRoot == null)
	    _objectStoreRoot = arjPropertyManager.propertyManager.getProperty(Environment.LOCALOSROOT);

	if (_objectStoreDir == null)
	{
	    _objectStoreDir = arjPropertyManager.propertyManager.getProperty(Environment.OBJECTSTORE_DIR);
	    if (_objectStoreDir == null || _objectStoreDir.length() == 0)
	    	_objectStoreDir = com.arjuna.ats.arjuna.common.Configuration.objectStoreRoot();

	    if (_objectStoreDir == null || _objectStoreDir.length() == 0)
		throw new ObjectStoreException(Environment.OBJECTSTORE_DIR+" not set.");
	
	    if (!_objectStoreDir.endsWith(File.separator))
		_objectStoreDir = _objectStoreDir + File.separator;

	    /*
	     * We use the classname of the object store implementation to
	     * specify the directory for the object store.
	     */
	
	    _objectStoreDir = _objectStoreDir + className().stringForm();
	}

	String toReturn = null;
	
	if ((localOSRoot == null) || (localOSRoot.length() == 0))
	{
	    if ((_objectStoreRoot != null) && (_objectStoreRoot.length() > 0))
		localOSRoot = _objectStoreRoot;
	    else
		localOSRoot = "defaultStore"+File.separator;
	}
    
	if ((localOSRoot != null) && (localOSRoot.length() > 0))
	    toReturn = _objectStoreDir + File.separator+localOSRoot;

	if (!toReturn.endsWith(File.separator))
	    toReturn = toReturn + File.separator;
	
	return toReturn;
    }
    
    /**
     * Does this store need to do the full write_uncommitted/commit protocol?
     */

public boolean fullCommitNeeded ()
    {
	return true;
    }

    /**
     * Is the current state of the object that provided as the last
     * parameter?
     */

public boolean isType (Uid u, String tn, int st) throws ObjectStoreException
    {
	return (((st & currentState(u, tn)) == st) ? true : false);
    }

    /**
     * Initialise the object store.
     */

public void initialise (Uid u, String tn)
    {
    }

    /**
     * Pack up the object store state. May be used to ship an entire
     * object store across the network, or persist a volatile object
     * store.
     */

public void pack (OutputBuffer buff) throws IOException
    {
    }

    /**
     * Unpack an object store.
     */

public void unpack (InputBuffer buff) throws IOException
    {
    }

public ClassName className ()
    {
	return ObjectStoreImple._className;
    }

public final int shareState ()
    {
	return shareStatus;
    }
    
public final String storeDir ()
    {
	return _objectStoreDir;
    }
    
public final String storeRoot ()
    {
	return _objectStoreRoot;
    }
    
public static ClassName name ()
    {
	return ObjectStoreImple._className;
    }
    
protected ObjectStoreImple ()
    {
	this(ObjectStore.OS_UNSHARED);
    }
    
protected ObjectStoreImple (int ss)
    {
	shareStatus = ss;
    }

protected ObjectStoreImple (ObjectName objName)
    {
	parseObjectName(objName);
    }
    
    /**
     * Suppress directories of the specified type from
     * allTypes etc?
     */

protected abstract boolean supressEntry (String name);

    /**
     * Given a type id which is possibly hidden (e.g., has a ! at the
     * end), make it a valid Uid so we can return it.
     */

protected String revealedId (String name)
    {
	return name;
    }

private final void parseObjectName (ObjectName objName)
    {
	if (objName != null)
	{
	    try
	    {
		_objectStoreRoot = objName.getStringAttribute(Environment.LOCALOSROOT);
	    }
	    catch (Exception e)
	    {
	    }

	    try
	    {
		_objectStoreDir = objName.getStringAttribute(Environment.OBJECTSTORE_DIR);
	    }
	    catch (Exception e)
	    {
	    }

	    try
	    {
		String shareType = objName.getStringAttribute(Environment.OBJECTSTORE_SHARE);

		if (shareType != null)
		{
		    if (shareType.equals("OS_SHARED"))
			shareStatus = ObjectStore.OS_SHARED;
		}
	    }
	    catch (Exception e)
	    {
	    }
	}
    }

protected int shareStatus; // is the implementation sharing states between VMs?

private String _objectStoreRoot = null;
private String _objectStoreDir = null;
    
private static final ClassName _className = new ClassName("ObjectStoreImple");
    
}
