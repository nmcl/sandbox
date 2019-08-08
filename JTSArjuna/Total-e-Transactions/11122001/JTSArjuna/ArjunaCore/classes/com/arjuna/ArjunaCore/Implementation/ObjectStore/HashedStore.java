/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HashedStore.java,v 1.1.4.1.2.2.2.2.2.3.112.1 2002/07/08 11:29:00 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import java.io.*;
import java.io.File;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.NumberFormatException;

/*
 * Should be derived from FragmentedStore, but we currently
 * don't have such an implementation in Java.
 */

/**
 * The basic shadowing store implementations store the object states in
 * a separate file within the same directory in the object store, determined
 * by the object's type. However, as the number of file entries within the
 * directory increases, so does the search time for finding a specific file.
 * The HashStore implementation hashes object states over many different
 * sub-directories to attempt to keep the number of files in a given
 * directory low, thus improving performance as the number of object states
 * grows.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: HashedStore.java,v 1.1.4.1.2.2.2.2.2.3.112.1 2002/07/08 11:29:00 nmcl Exp $
 * @since JTS 2.0.
 */

public class HashedStore extends ShadowNoFileLockStore
{

public int typeIs ()
    {
	return ObjectStoreType.HASHED;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_HashedStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_HashedStore();
    }

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */

public static ShadowingStore create ()
    {
	return new HashedStore("");
    }

public static ShadowingStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String location = (String) param[0];
	Integer shareStatus = (Integer) param[1];
	int ss = ObjectStore.OS_UNSHARED;

	if (shareStatus != null)
	{
	    try
	    {
		ss = shareStatus.intValue();
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("HashedStore.create caught: "+e);
	    }
	}

	return new HashedStore(location, ss);
    }

    /**
     * Given a type name initialise <code>state</code> to contains all of the
     * Uids of objects of that type
     */

public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.allObjUids("+tName+", "+state+", "+match+")");
	}

	/*
	 * Directory ALWAYS has a trailing '/'
	 */

	String directory = locateStore(getStoreName());
	OutputObjectState store = new OutputObjectState();

	/* Does typename start with a '/' if so skip over */

	if ((tName != null) && (tName.charAt(0) == File.separatorChar))
	    directory = directory + tName.substring(1, tName.length());
	else
	    directory = directory + tName;

	if (!directory.endsWith(File.separator))
	    directory = directory + File.separator;

	File f = new File(directory);
	String[] entry = f.list();

	if ((entry != null) && (entry.length > 0))
	{
	    for (int i = 0; i < entry.length; i++)
	    {
		if ( Character.isDigit(entry[i].charAt(0))) 
		{
		    File dir = new File(directory + entry[i]);
		    if (dir.isDirectory())
		    {
			String[] dirEnt = dir.list();
    
			for (int j = 0; j < dirEnt.length; j++)
			{
			    try
			    {
				Uid aUid = new Uid(dirEnt[j]);
				if ((aUid.notEquals(Uid.nullUid())) && ((match == ObjectStore.OS_UNKNOWN) ||
				    (isType(aUid, tName, match))))
				{
				    aUid.pack(store);
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
				throw new ObjectStoreException("HashedStore.allObjUids - could not pack Uid.");
			    }
			}
		    }
		}
		else
		{
		    // ignore
		}
	    }
	}

	/* terminate list */

	try
	{
	    Uid.nullUid().pack(store);
	}
	catch (IOException e)
	{
	    throw new ObjectStoreException("HashedStore.allObjUids - could not pack end of list Uid.");
	}

	state.setBuffer(store.buffer());

	store = null;

	return true;
    }

    /*
     * Protected constructors and destructor
     */

protected HashedStore ()
    {
	this(ObjectStore.OS_UNSHARED);
    }

protected HashedStore (int shareStatus)
    {
	super(shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.HashedStore()");
	}
    }

protected HashedStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }

protected HashedStore (String locationOfStore, int shareStatus)
    {
	super(shareStatus);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.HashedStore("+locationOfStore+")");
	}

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println(e.getMessage());

	    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
	}
    }

    /**
     * @return the file name for the state of the object
     * identified by the Uid and TypeName. 
     */

protected String genPathName (Uid objUid, String tName, int otype) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "HashedStore.genPathName("+objUid+", "+tName+", "+ObjectStore.stateTypeString(otype)+")");
	}

	String storeName = locateStore(getStoreName());
	String fname = null;
	String cPtr = null;
	int uidHash = objUid.hashCode();
	String uidString = objUid.toString();
	String hashDir = uidHash % HashedStore.NUMBEROFDIRECTORIES + File.separator;

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
	    {
		cPtr = cPtr.replace(FileSystemStore.unixSeparator, File.separatorChar);
	    }
	}

	/*
	 * storeName always ends in '/' so we can remove any
	 * at the start of the type name.
	 */

	if (cPtr.charAt(0) == File.separatorChar)
	    cPtr = cPtr.substring(1, cPtr.length());

	if (cPtr.charAt(cPtr.length() -1) != File.separatorChar)
	    fname = storeName + cPtr + File.separator + hashDir + os;
	else
	    fname = storeName + cPtr + hashDir + os;

	/*
	 * Make sure we don't end in a '/'.
	 */

	if (fname.charAt(fname.length() -1) == File.separatorChar)
	    fname = fname.substring(0, fname.length() -2);

	// mark the shadow copy distinctly
	if (otype == ObjectStore.OS_SHADOW)
	    fname = fname + SHADOWCHAR;

	return fname;
    }

    public static final char SHADOWCHAR = '!';

    private static final int DEFAULT_NUMBER_DIRECTORIES = 255;
    private static int NUMBEROFDIRECTORIES = DEFAULT_NUMBER_DIRECTORIES;

    static
	{
	    String numberOfDirs = PropertyManager.getProperty(ArjunaCoreEnvironment.HASHED_DIRECTORIES);

	    if (numberOfDirs != null)
	    {
		try
		{
		    Integer i = new Integer(numberOfDirs);

		    NUMBEROFDIRECTORIES = i.intValue();

		    if (NUMBEROFDIRECTORIES <= 0)
		    {
			ErrorStream.stream(ErrorStream.WARNING).println("HashedStore - invalid number of hash directories: "+numberOfDirs+". Will use default.");

			NUMBEROFDIRECTORIES = DEFAULT_NUMBER_DIRECTORIES;
		    }
		}
		catch (NumberFormatException e)
		{
		    ErrorStream.warning().println("HashedStore - invalid number of hash directories: "+numberOfDirs);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError("Invalid hash directory number: "+numberOfDirs);
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println("HashedStore caught exception: "+e);

		    throw new com.arjuna.ArjunaCommon.Common.FatalError(e.toString());
		}
	    }
	}

}

