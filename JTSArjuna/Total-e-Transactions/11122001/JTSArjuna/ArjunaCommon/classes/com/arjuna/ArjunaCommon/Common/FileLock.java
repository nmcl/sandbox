/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: FileLock.java,v 1.2.4.1.6.1.22.1.4.2.34.1.4.3 2001/07/19 13:53:47 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.io.*;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.InterruptedException;

/**
 * Sometimes it is necessary to lock a file at the disk level.
 *
 * Since there is no native Java way of locking a file, we
 * have to implement our own. Unfortunately, it appears as though
 * we can only assume that rename is atomic. We base the locking on
 * this then: rename the lock file and update it with the lock owners.
 *
 * How it works:
 *
 * for every file we want to lock we create an _lock file. This file
 * contains information about who is locking the file, and in what mode.
 * (Single writer, multiple readers.) To guarantee atomicity of update, we
 * move (rename) the actual file each time we want to lock it and update the
 * lock file. When this is done, we move (rename) it back. Almost like a
 * two-phase commit protocol! Currently we don't support re-entrant locking.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FileLock.java,v 1.2.4.1.6.1.22.1.4.2.34.1.4.3 2001/07/19 13:53:47 nmcl Exp $
 * @since JTS 1.0.
 */

public class FileLock
{

public static final int F_RDLCK = 0;
public static final int F_WRLCK = 1;

public static final int defaultTimeout = 10;  // milliseconds
public static final int defaultRetry = 10;
     
public FileLock (File name)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "FileLock ( "+name+" )");
	}

	_theFile = name;
	_lockFile = new File(name.toString() + "_lock");
	_lockFileLock = new File(name.toString() + "_lock.lock");
	_timeout = FileLock.defaultTimeout;
	_retry = FileLock.defaultRetry;
    }

public FileLock (File name, long timeout, long retry)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "FileLock ( "+name+", "+timeout+", "+retry+" )");
	}

	_theFile = name;
	_lockFile = new File(name.toString() + "_lock");
	_lockFileLock = new File(name.toString() + "_lock.lock");
	_timeout = timeout;
	_retry = retry;
    }

public void finalize ()
    {
	_theFile = null;
    }
    
    /**
     * @since JTS 2.1.1.
     */

public boolean lock (int lmode)
    {
	return lock(lmode, false);
    }
    
public boolean lock (int lmode, boolean create)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "FileLock.lock called for "+_lockFile);
	}

	if (create && !_theFile.exists())
	{
	    createFile();
	}
	
	/*
	 * If the lock file exists, and the mode is exclusive, then we can
	 * immediately return false.
	 *
	 * Currently we do not implement re-entrant locking, which requires
	 * some owner id.
	 */

	if (_lockFile.exists() && (lmode == FileLock.F_WRLCK))
	    return false;

	int number = 0;

	if (lockFile())  // have we moved the file (if it exists)?
	{
	    try
	    {
		DataInputStream ifile = new DataInputStream(new FileInputStream(_lockFile));
		int value = ifile.readInt();
	    
		/*
		 * Already exclusively locked.
		 */
	    
		if (value == FileLock.F_WRLCK)
		{
		    ifile.close();
		    unlockFile();
		    
		    return false;
		}
		else
		    number = ifile.readInt();

		ifile.close();
	    }
	    catch (FileNotFoundException e)
	    {
	    }
	    catch (IOException e)
	    {
		/*
		 * Something went wrong. Abandon.
		 */

		unlockFile();

		return false;
	    }

	    try
	    {
		DataOutputStream ofile = new DataOutputStream(new FileOutputStream(_lockFile));

		number++;
	
		ofile.writeInt(lmode);
		ofile.writeInt(number);

		ofile.close();

		unlockFile();
	
		return true;
	    }
	    catch (IOException e)
	    {
		/*
		 * Something went wrong. Abandon.
		 * Lock file is ok since we haven't touched it.
		 */

		unlockFile();

		return false;
	    }
	}

	return false;
    }

public boolean unlock ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "FileLock.unlock called for "+_lockFile);
	}

	if (!_lockFile.exists())
	    return false;

	if (lockFile())
	{
	    int number = 0, mode = 0;
	    
	    try
	    {
		DataInputStream ifile = new DataInputStream(new FileInputStream(_lockFile));

		mode = ifile.readInt();
		number = ifile.readInt();
		ifile.close();

		number--;

		if (number == 0)
		{
		    _lockFile.delete();

		    unlockFile();

		    return true;
		}
	    }
	    catch (FileNotFoundException e)
	    {
		unlockFile();

		return false;
	    }
	    catch (IOException e)
	    {
		unlockFile();

		return false;
	    }
	    
	    try
	    {
		DataOutputStream ofile = new DataOutputStream(new FileOutputStream(_lockFile));

		ofile.writeInt(mode);
		ofile.writeInt(number);
		ofile.close();
		
		unlockFile();
		
		return true;
	    }
	    catch (IOException e)
	    {
		unlockFile();
		
		return false;
	    }
	}

	return false;
    }

public static String modeString (int mode)
    {
	switch (mode)
	{
	case FileLock.F_RDLCK:
	    return "FileLock.F_RDLCK";
	case FileLock.F_WRLCK:
	    return "FileLock.F_WRLCK";
	default:
	    return "Unknown";
	}
    }

private final boolean createFile ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_GENERAL, "FileLock.createFile called for "+_lockFile);
	}

	byte b[] = new byte[1];

	try
	{
	    if (!_theFile.exists())
	    {
		_theFile.createNewFile();

		return true;
	    }
	    else
		return false;
	}
	catch (IOException e)
	{
	    ErrorStream.warning().println("An error occurred while creating file "+_lockFile);

	    return false;
	}	
    }
    
private final boolean lockFile ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_GENERAL, "FileLock.lockFile called for "+_lockFile);
	}

	for (int i = 0; i < _retry; i++)
	{
	    try
	    {
		if (_lockFileLock.createNewFile())
		    return true;
		else
		{
		    try
		    {
			Thread.sleep(_timeout);
		    }
		    catch (InterruptedException e)
		    {
		    }
		}
	    }
	    catch (IOException ex)
	    {
		// already created, so locked!
	    }
	}

	return false;
    }

private final boolean unlockFile ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS,
						 VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_GENERAL, "FileLock.unlockFile called for "+_lockFile);
	}

	return _lockFileLock.delete();
    }

private File _theFile;
private File _lockFile;
private File _lockFileLock;
private long _timeout;
private long _retry;
    
}
