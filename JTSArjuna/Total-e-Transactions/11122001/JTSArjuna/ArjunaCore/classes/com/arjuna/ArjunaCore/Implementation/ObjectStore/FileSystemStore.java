/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: FileSystemStore.java,v 1.1.4.1.2.3.2.2.2.2.4.1.8.3.30.2.66.1 2002/11/20 12:13:40 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;
import java.util.Hashtable;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * The basic class for file system object stores. This is not actually
 * an object store implementation, since other classes must provide
 * implementations of the abstract methods. It does provide implementations
 * of common methods though.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FileSystemStore.java,v 1.1.4.1.2.3.2.2.2.2.4.1.8.3.30.2.66.1 2002/11/20 12:13:40 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class FileSystemStore extends ObjectStoreImple
{

public FileSystemStore (String locationOfStore, int ss)
    {
	super(ss);
	
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.FileSystemStore("+locationOfStore+")");
	}

	fullStoreName = null;
	localStoreName = null;
	isValid = true;

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    isValid = false;

	    ErrorStream.warning().println(e.getMessage());

	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}
    }

public String getStoreName ()
    {
	return localStoreName;
    }

    /*
     * read an uncommitted instance of State out of the object store. 
     * The instance is identified by the unique id and type
     */

public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.read_committed("+storeUid+", "+tName+")");
	}

	return read_state(storeUid, tName, ObjectStore.OS_ORIGINAL);
    }

public InputObjectState read_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.read_uncommitted("+storeUid+", "+tName+")");
	}

	return read_state(storeUid, tName, ObjectStore.OS_SHADOW);
    }

public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.remove_committed("+storeUid+", "+tName+")");
	}

	return remove_state(storeUid, tName, ObjectStore.OS_ORIGINAL);
    }

public boolean remove_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.remove_uncommitted("+storeUid+", "+tName+")");
	}

	return remove_state(storeUid, tName, ObjectStore.OS_SHADOW);
    }

public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.write_committed("+storeUid+", "+tName+")");
	}

	return write_state(storeUid, tName, state, ObjectStore.OS_ORIGINAL);
    }

public boolean write_uncommitted (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.write_uncommitted("+storeUid+", "+tName+", "+state+")");
	}

	return write_state(storeUid, tName, state, ObjectStore.OS_SHADOW);
    }

public final synchronized boolean storeValid ()
    {
	return isValid;
    }

public final synchronized void makeInvalid ()
    {
	isValid = false;
    }
    
public final synchronized void makeValid ()
    {
	isValid = true;
    }
    
    /**
     * Given a type name initialise the <code>state</code> to contains all of
     * the Uids of objects of that type
     */

public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.allObjUids("+tName+", "+state+", "+match+")");
	}

	String directory = null;
	OutputObjectState store = new OutputObjectState();
	
	/*
	 * If typename starts with a '/' then skip over it.
	 */
	
	if ((tName != null) && (tName.charAt(0) == File.separatorChar))
	{
	    String s = tName.substring(1, tName.length());
	    directory = new String(fullStoreName + s);
	}
	else
	    directory = new String(fullStoreName + tName);

	File f = new File(directory);
	String[] entry = f.list();
    
	if ((entry != null) && (entry.length > 0))
	{
	    for (int i = 0; i < entry.length; i++)
	    {
		try
		{
		    Uid aUid = new Uid(entry[i], true);

		    if (!aUid.valid() || (aUid.equals(Uid.nullUid())))
		    {
			if (present(revealedId(entry[i]), entry))
			    aUid = null;
		    }

		    if ((aUid != null) && (aUid.valid()))
		    {
			if ((aUid.notEquals(Uid.nullUid())) && ((match == ObjectStore.OS_UNKNOWN) ||
								(isType(aUid, tName, match))))
			{
			    aUid.pack(store);
			}
		    }
		}
		catch (NumberFormatException e)
		{
		    /*
		     * Not a number at start of file.
		     */
		}
		catch (IOException e)
		{
		    throw new ObjectStoreException("FileSystemStore::allObjUids - could not pack Uid.");
		}
	    }
	}

	try
	{
	    Uid.nullUid().pack(store);
	}
	catch (IOException e)
	{
	    throw new ObjectStoreException("FileSystemStore::allObjUids - could not pack end of list Uid.");
	}

	state.setBuffer(store.buffer());

	store = null;

	return true;
    }
 
public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.allTypes("+foundTypes+")");
	}

	boolean result = true;
	String directory = new String(fullStoreName);
	File f = new File(directory);
	String[] entry = f.list();

	if (entry == null)
	    return true;

	OutputObjectState store = new OutputObjectState();
	
	for (int i = 0; i < entry.length; i++)
	{
	    if (!supressEntry(entry[i]))
	    {
		File tmpFile = new File(directory+File.separator+entry[i]);
		
		if (tmpFile.isDirectory())
		{
		    try
		    {
			store.packString(entry[i]);

			result = allTypes(store, entry[i]);
		    }
		    catch (IOException e)
		    {
			throw new ObjectStoreException("FileSytemStore::allTypes - could not pack entry string.");
		    }
		}

		tmpFile = null;
	    }
	}

	try
	{
	    store.packString("");
	}
	catch (IOException e)
	{
	    throw new ObjectStoreException("FileSystemStore::allTypes - could not pack end of list string.");
	}

	foundTypes.setBuffer(store.buffer());

	return result;
    }

public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	if (localStoreName != null)
	{
	    if (localStoreName.compareTo("") == 0)
	    {
		buff.packString(null);
		return;
	    }
	}

	buff.packString(localStoreName);
    }

public synchronized void unpackFrom (InputBuffer buff) throws IOException
    {
	try
	{
	    setupStore(buff.unpackString());
	}
	catch (ObjectStoreException e)
	{
	    throw new IOException("FileSystemStore::setupStore - error from unpack object store.");
	}
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_FileSystemStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_FileSystemStore();	
    }    
    
protected abstract InputObjectState read_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean remove_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean write_state (Uid u, String tn, OutputObjectState buff, int s) throws ObjectStoreException;

    /**
     * Turn file sync on and off.
     */
    
protected synchronized final void syncOn ()
    {
	doSync = true;
    }
    
protected synchronized final void syncOff ()
    {
	doSync = false;
    }

    /**
     * Are synchronous write enabled?
     */

protected synchronized final boolean synchronousWrites ()
    {
	return doSync;
    }

    /**
     * Lock the file in the object store.
     */

protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	FileLock fileLock = new FileLock(fd);

	return fileLock.lock(lmode, create);
    }

    /**
     * Unlock the file in the object store.
     */

protected synchronized boolean unlock (File fd)
    {
	FileLock fileLock = new FileLock(fd);

	return fileLock.unlock();
    }
    
protected FileSystemStore (int ss)
    {
	super(ss);
	
	fullStoreName = null;
	localStoreName = null;
	isValid = true;
    }

protected boolean allTypes (OutputObjectState foundTypes, String root) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.allTypes("+foundTypes+", "+root+")");
	}

	boolean result = true;
	String directory = new String(fullStoreName + File.separator + root);
	File f = new File(directory);
	String[] entry = f.list();

	if ((entry != null) && (entry.length > 0))
	{
	    for (int i = 0; i < entry.length; i++)
	    {
		if (!supressEntry(entry[i]))
		{
		    try
		    {
			File tmpFile = new File(directory+File.separator+entry[i]);

			if (tmpFile.isDirectory())
			{
			    foundTypes.packString(root+File.separator+entry[i]);
			
			    result = allTypes(foundTypes, root+File.separator+entry[i]);
			}

			tmpFile = null;
		    }
		    catch (IOException e)
		    {
			throw new ObjectStoreException("FileSystemStore::allTypes - could not pack entry string.");
		    }
		}
	    }
	}

	return result;
    }
    
    /**
     * @return the file name for the state of the object
     * identified by the Uid and TypeName. If the StateType argument
     * is OS_SHADOW then the Uid part of the name includes # characters.
     * The magic number SLOP below is the number of extra characters needed
     * to make up the entire path.
     */

protected String genPathName (Uid objUid,
			      String tName, int m) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.genPathName("+objUid+", "+tName+", "+m+")");
	}

	String storeName = locateStore(getStoreName());
	String cPtr = null;
	String fname = null;
	String uidString = objUid.toString();
	
	/*
	 * Now convert all ':' to '_' to be portable across file systems.
	 */

	String os = uidString.replace(':', '_');

	uidString = null;
	
	if ((tName == null) || (tName.length() == 0))
	    cPtr = "";
	else
	{
	    cPtr = tName;

	    /*
	     * Convert Unix separators to 'other', i.e., Windows!
	     */

	    if (FileSystemStore.rewriteSeparator && (cPtr.indexOf(FileSystemStore.unixSeparator) != -1))
		cPtr = cPtr.replace(FileSystemStore.unixSeparator, File.separatorChar);
	}

	/*
	 * storeName always ends in '/' so we can remove any
	 * at the start of the type name.
	 */
	
	if (cPtr.charAt(0) == File.separatorChar)
	    cPtr = cPtr.substring(1, cPtr.length());

	if (cPtr.charAt(cPtr.length() -1) != File.separatorChar)
	    fname = storeName + cPtr + File.separator + os;
	else
	    fname = storeName + cPtr + os;

	/*
	 * Make sure we don't end in a '/'.
	 */
	
	if (fname.charAt(fname.length() -1) == File.separatorChar)
	    fname = fname.substring(0, fname.length() -2);

	return fname;
    }

protected synchronized boolean setupStore (String location) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.setupStore("+location+")");
	}

	isValid = true;
    
	if (location == null)
	    location = "";

	localStoreName = location;
	fullStoreName = locateStore(localStoreName);

	/* The root of the objectstore must exist and be writable */

	if ((fullStoreName == null) || !createHierarchy(fullStoreName))
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("FileSystemStore::setupStore - cannot access root of object store: "+fullStoreName);
	    isValid = false;
	}

	return isValid;
    }

protected boolean supressEntry (String name)
    {
	if ((name.compareTo(".") == 0) || (name.compareTo("..") == 0))
	    return true;
	else
	    return false;
    }

    /**
     * Attempt to build up the object store in the file system dynamically.
     * This creates directories as required as new types are added to the
     * store. Note that we made sure the root object store was created and
     * writable at construction time
     */

protected synchronized final boolean createHierarchy (String path) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileSystemStore.createHierarchy("+path+")");
	}

	if ((path != null) && (path.length() > 0))
	{
	    File f = null;

	    /*
	     * Is string a complete directory list, or is it an
	     * absolute file name?
	     */
	    
	    if (path.charAt(path.length() -1) != File.separatorChar)
	    {
		int index = path.lastIndexOf(File.separator);

		if (index <= 0)
		    return true;
		else
		    f = new File(path.substring(0, index));
	    }
	    else
		f = new File(path);

	    if (f.exists())
		return true;
	    else
	    {
		if (!f.mkdirs())
		    return f.exists();
		else
		    return true;
	    }
	}
	else
	    throw new ObjectStoreException("FileSystemStore::createHierarchy - null directory name.");
    }

    /**
     * If this object store implementation is exclusively working on a set of
     * object states, then this method will check a file cache first.
     * Otherwise, we must go back to the file system each time to check the
     * status of the file.
     *
     * If we add a FileDescriptor cache a la the C++ version then we would
     * be able to get rid of the state cache and simply check to see if we
     * had a fd for it.
     */

protected synchronized final boolean exists (String path)
    {
	if (super.shareStatus == ObjectStore.OS_UNSHARED)
	{
	    if (FileSystemStore.fileCache.get(path) != null)
		return true;
	}
	
	/*
	 * If here then we need to check the file system. If there, we will
	 * put it into the cache (if appropriate).
	 */

	File f = new File(path);
	boolean doesExist = f.exists();

	if (doesExist)
	{
	    addToCache(path);
	}

	return doesExist;
    }

protected synchronized final void addToCache (String fname)
    {
	if (super.shareStatus == ObjectStore.OS_UNSHARED)
	{
	    FileSystemStore.fileCache.put(fname, fname);
	}
    }

protected synchronized final void removeFromCache (String fname)
    {
	removeFromCache(fname, true);
    }
    
    /**
     * Print a warning if the file to be removed is not in the cache.
     *
     * @since 2.1.1.
     */

protected synchronized final void removeFromCache (String fname, boolean warn)
    {
	if (super.shareStatus == ObjectStore.OS_UNSHARED)
	{
	    if ((FileSystemStore.fileCache.remove(fname) == null) && warn)
		ErrorStream.warning().println("FileSystemStore.removeFromCache - no entry for "+fname);
	}
    }

private final boolean present (String id, String[] list)
    {
	for (int i = 0; i < list.length; i++)
	{
	    if (list[i].equals(id))
		return true;
	}
	
	return false;
    }
    
static final char unixSeparator = '/';

static boolean rewriteSeparator = false;
    
private String  fullStoreName;
private String  localStoreName;
private boolean isValid;
    
    /*
     * These values should be determined via something like pathconf
     */

private static final int MAXPNAMELEN = 255;
private static final int MAXNAMELENGTH = 255;
private static final int SLOP = 9;

private static boolean   doSync = true;
private static Hashtable fileCache = new Hashtable();

    static
    {
	String syncOpt = PropertyManager.getProperty(ArjunaCoreEnvironment.OBJECTSTORE_SYNC);

	if (syncOpt != null)
	{
	    if (syncOpt.compareTo("OFF") == 0)
		FileSystemStore.doSync = false;
	}
	else
	    FileSystemStore.doSync = true;

	if (File.separatorChar != FileSystemStore.unixSeparator)
	    rewriteSeparator = true;
    }

}
