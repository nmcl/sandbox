/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HashedStore.java,v 1.1 2000/02/25 14:04:09 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Common.ArjunaLiteEnvironment;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaLite.Interface.ObjectStore;
import java.io.*;
import java.io.File;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.NumberFormatException;

/*
 * Should be derived from FragmentedStore, but we currently
 * don't have such an implementation in Java.
 */

public class HashedStore extends ShadowNoFileLockStore
{

public int typeIs ()
    {
	return ObjectStoreType.HASHED;
    }

public ClassName className ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_HashedStore();
    }

public static ClassName name ()
    {
	return ArjunaLiteNames.Implementation_ObjectStore_HashedStore();
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

	return new HashedStore(location);
    }

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException
    {
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

	if (!directory.endsWith("/"))
	    directory = directory + "/";

	File f = new File(directory);
	String[] entry = f.list();

	if ((entry != null) && (entry.length > 0))
	{
	    for (int i = 0; i < entry.length; i++)
	    {
		File dir = new File(entry[i]);

		if (dir.isDirectory())
		{
		    String[] dirEnt = dir.list();

		    for (int j = 0; j < dirEnt.length; j++)
		    {
			try
			{
			    Uid aUid = new Uid(entry[i]);

			    if ((aUid.notEquals(Uid.nullUid())) && ((match == ObjectStore.OS_UNKNOWN)) ||
				(isType(aUid, tName, match)))
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
	super();
    }

protected HashedStore (String locationOfStore)
    {
	super();

	try
	{
	    setupStore(locationOfStore);
	}
	catch (ObjectStoreException e)
	{
	    System.out.println(e.getMessage());
	    System.exit(0);
	}
    }
    
    /*
     * Function to return the file name for the state of the object
     * identified by the Uid and TypeName. 
     * Return char * is dynamically allocated and must be freed by caller
     */

protected String genPathName (Uid objUid, String tName, int m) throws ObjectStoreException
    {
	String storeName = locateStore(getStoreName());
	String fname = null;
	String cPtr = null;
	int uidHash = objUid.hashCode();
	String uidString = objUid.toString();
	String hashDir = uidHash % HashedStore.NUMBEROFDIRECTORIES + "/" + uidString;

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
	    fname = storeName + cPtr + File.separator + hashDir;
	else
	    fname = storeName + cPtr + hashDir;
	
	/*
	 * Make sure we don't end in a '/'.
	 */
	
	if (fname.charAt(fname.length() -1) == File.separatorChar)
	    fname = fname.substring(0, fname.length() -2);

	return fname;
    }

private static final int DEFAULT_NUMBER_DIRECTORIES = 255;
private static int NUMBEROFDIRECTORIES = DEFAULT_NUMBER_DIRECTORIES;

    static
	{
	    String numberOfDirs = PropertyManager.getProperty(ArjunaLiteEnvironment.HASHED_DIRECTORIES);

	    if (numberOfDirs != null)
	    {
		try
		{
		    Integer i = new Integer(numberOfDirs);

		    NUMBEROFDIRECTORIES = i.intValue();

		    if (NUMBEROFDIRECTORIES <= 0)
		    {
			System.err.println("WARNING - HashedStore - invalid number of hash directories: "+numberOfDirs);
			System.err.println("WARNING - Will use default.");

			NUMBEROFDIRECTORIES = DEFAULT_NUMBER_DIRECTORIES;
		    }
		}
		catch (NumberFormatException e)
		{
		    System.err.println("HashedStore - invalid number of hash directories: "+numberOfDirs);
		    System.exit(0);
		}
		catch (Exception e)
		{
		    System.err.println("HashedStore caught exception: "+e);
		    System.exit(0);
		}
	    }
	}
    
};
