/*
 * Copyright (C) 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileLock.java,v 1.2 1998/07/06 13:30:45 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import java.io.*;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.InterruptedException;

/*
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
 */

public class FileLock
{

public static final int F_RDLCK = 0;
public static final int F_WRLCK = 1;

public static final int defaultTimeout = 100;  // milliseconds
public static final int defaultRetry = 10;
     
public FileLock (File name)
    {
	_theFile = name;
	_lockFile = new File(new String(name.toString()+"_lock"));
	_shadowFile = new File(new String(name.toString()+"#"));
	_lockFileShadow = new File(new String(name.toString()+"_lock#"));
	_timeout = FileLock.defaultTimeout;
	_retry = FileLock.defaultRetry;
    }

public FileLock (File name, long timeout, long retry)
    {
	_theFile = name;	
	_lockFile = new File(new String(name.toString()+"_lock"));
	_shadowFile = new File(new String(name.toString()+"#"));	
	_lockFileShadow = new File(new String(name.toString()+"_lock#"));
	_timeout = timeout;
	_retry = retry;
    }

public boolean createFile ()
    {
	byte b[] = new byte[1];

	try
	{
	    DataOutputStream ofile = new DataOutputStream(new FileOutputStream(_theFile));

	    if (!_theFile.exists())
	    {
		ofile.write(b, 0, 0);  // create zero-length file

		return true;
	    }
	    else
		return false;
	}
	catch (IOException e)
	{
	    System.out.println("An error occurred while creating file "+_theFile);
	    return false;
	}	
    }
    
public boolean lock (int lmode, boolean create)
    {
	if (create && !_theFile.exists())
	{
	    createFile();
	}
	
	/*
	 * If the lock file exists, and the mode is exclusive, then we can
	 * immediately return false.
	 *
	 * Currently we do not implement re-entrant locking, which requires some
	 * owner id.
	 */

	if (_lockFile.exists() && _lockFile.isFile())
	{
	    if (lmode == FileLock.F_WRLCK)
		return false;
	}

	int number = 0;

	if (grabFile())  // have we moved the file (if it exists)?
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
		    releaseFile();
		    
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

		releaseFile();

		return false;
	    }

	    try
	    {
		DataOutputStream ofile = new DataOutputStream(new FileOutputStream(_lockFileShadow));

		number++;
	
		ofile.writeInt(lmode);
		ofile.writeInt(number);

		ofile.close();

		_lockFileShadow.renameTo(_lockFile);
		
		releaseFile();
	
		return true;
	    }
	    catch (IOException e)
	    {
		/*
		 * Something went wrong. Abandon. Lock file is ok since we haven't touched it.
		 */

		releaseFile();

		_lockFileShadow.delete();  // just in case!

		return false;
	    }
	}

	return false;
    }

public boolean unlock ()
    {
	if (!_lockFile.exists() && _lockFile.isFile())
	    return false;

	/*
	 * Has the file been deleted. If so, simply remove any remaining
	 * locking file.
	 */
	
	if (!_theFile.exists())
	{
	    if (_lockFile.exists())
		_lockFile.delete();

	    return true;
	}
	
	if (grabFile())
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
		    releaseFile();

		    return true;
		}
	    }
	    catch (FileNotFoundException e)
	    {
		releaseFile();

		return false;
	    }
	    catch (IOException e)
	    {
		releaseFile();

		return false;
	    }
	    
	    try
	    {
		DataOutputStream ofile = new DataOutputStream(new FileOutputStream(_lockFileShadow));

		ofile.writeInt(mode);
		ofile.writeInt(number);
		ofile.close();

		_lockFileShadow.renameTo(_lockFile);

		releaseFile();
		
		return true;
	    }
	    catch (IOException e)
	    {
		releaseFile();
		
		_lockFileShadow.delete();
		
		return false;
	    }
	}

	return false;
    }

private boolean grabFile ()
    {
	for (int i = 0; i < _retry; i++)
	{
	    /*
	     * If the shadow exists then someone else is in the
	     * process of modifying the lock data. Wait.
	     */
		
	    if (_shadowFile.exists())
	    {
		if (!_shadowFile.isFile())  // not a file!!
		    return false;

		try
		{
		    Thread.sleep(_timeout);
		}
		catch (InterruptedException e)
		{
		}
	    }
	    else
	    {
		if (_theFile.renameTo(_shadowFile))
		{
		    return true;
		}
	    }
	}

	return false;
    }

private boolean releaseFile ()
    {
	return _shadowFile.renameTo(_theFile);
    }
    
    
private File _theFile;
private File _shadowFile;
private File _lockFile;
private File _lockFileShadow;
private long _timeout;
private long _retry;
    
};


