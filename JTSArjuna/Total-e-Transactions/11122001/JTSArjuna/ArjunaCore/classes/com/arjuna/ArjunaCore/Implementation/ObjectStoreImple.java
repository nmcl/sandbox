/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreImple.java,v 1.1.4.1.2.2.2.2.2.2.10.1 2001/03/07 15:09:10 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Common.Configuration;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.File;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
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
 * @version $Id: ObjectStoreImple.java,v 1.1.4.1.2.2.2.2.2.2.10.1 2001/03/07 15:09:10 nmcl Exp $
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
     * @return the full path of the store. location is always a relative name
     * NOTE this path always ends in a /
     */

public String locateStore (String location) throws ObjectStoreException
    {
	if (_objectStoreRoot == null)
	    _objectStoreRoot = PropertyManager.getProperty(ArjunaCoreEnvironment.LOCALOSROOT);

	if (_objectStoreDir == null)
	{
	    _objectStoreDir = PropertyManager.getProperty(ArjunaCoreEnvironment.OBJECTSTORE_DIR,
							  com.arjuna.ArjunaCore.Common.Configuration.objectStoreRoot());

	    if (_objectStoreDir == null)
		throw new ObjectStoreException(ArjunaCoreEnvironment.OBJECTSTORE_DIR+" not set.");
	
	    if (!_objectStoreDir.endsWith(File.separator))
		_objectStoreDir = _objectStoreDir + File.separator;

	    /*
	     * We use the classname of the object store implementation to
	     * locate specify the directory for the object store.
	     */
	
	    _objectStoreDir = _objectStoreDir + className().stringForm();
	}

	String toReturn = null;
	
	if ((location == null) || (location.length() == 0))
	{
	    if ((_objectStoreRoot != null) && (_objectStoreRoot.length() > 0))
		location = _objectStoreRoot;
	    else
		location = "defaultStore"+File.separator;
	}
    
	if ((location != null) && (location.length() > 0))
	    toReturn = _objectStoreDir + File.separator+location;

	if (!_objectStoreDir.endsWith(File.separator))
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

protected int shareStatus; // is the implementation sharing states between VMs?
    
private String _objectStoreRoot = null;
private String _objectStoreDir = null;
    
private static final ClassName _className = new ClassName("ObjectStoreImple");
    
}
